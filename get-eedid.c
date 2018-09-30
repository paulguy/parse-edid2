#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <linux/i2c-dev.h>
#include <getopt.h>

#include "eedid.h"

const char *default_template = "edid{}.bin";

typedef struct {
    unsigned char data[256];
    int unique;
} eedidblock;

typedef enum {
    SMBUS,
    EEDID
} opmode;

void usage() {
    fprintf(stderr, "USAGE: get-eedid [-s] [-d] [-o <output template>] <device path>\n"
                    "-s      SMBus mode.\n"
                    "-d      DDC2B mode.\n"
                    "-o      Output template, in DDC2B mode '{}' will be replaced\n"
                    "    with the page number.  In SMBus mode, it'll just be omitted.\n"
                    "<device path> Path to i2c device.\n");
}

int main(int argc, char **argv) {
    int i2cdev;
    opmode mode = SMBUS;
    const char *template = default_template;
    int namelen;
    const char *repchar;
    int reppos = 0;
    const char *devpath;
    FILE *out;
    eedidblock blocks[256];
    int i, j;
    int c;
    int scanopts = 1;

    while (scanopts) {
        int option_index = 0;
        static struct option long_options[] = {
            {"smbus",           no_argument,        0, 's'},
            {"ddc2b",           no_argument,        0, 'd'},
            {"output-template", required_argument,  0, 'o'},
            {0,                 0,                  0, 0}
        };

        c = getopt_long(argc, argv, "sdo:",
                        long_options, &option_index);
        if(c == -1) break;

        switch(c) {
            case 's':
                mode = SMBUS;
                break;
            case 'd':
                mode = EEDID;
                break;
            case 'o':
                template = optarg;
                break;
            default:
                scanopts = 0;
                break;
        }
    }

    /* bad argumenbt or no extra argument */
    if(scanopts == 0 || argc - optind != 1) {
        usage();
        exit(EXIT_FAILURE);
    }
    devpath = argv[optind];

    namelen = strlen(template);
    repchar = template;

    while(1) {
        repchar = strchr(repchar, '{');
        if(repchar == NULL) {
            if(mode == EEDID) {
                usage();
                exit(EXIT_FAILURE);
            } else {
                break;
            }
        }
        if(repchar[1] == '}') {
            reppos = repchar - template;
            break;
        }
    }

    i2cdev = open_i2c_dev(devpath);
    if(i2cdev < 0) {
        goto error0;
    }

    if(mode == SMBUS) {
        if(read_edid(i2cdev, blocks[0].data) < 0) {
            fprintf(stderr, "Couldn't read EDID.\n");
            goto error1;
        }

        char filename[namelen + 1];
        if(repchar == NULL) {
            strncpy(filename, template, namelen + 1);
        } else {
            memcpy(filename, template, reppos);
            memcpy(&(filename[reppos]), &(repchar[2]), namelen - reppos - 2);
            filename[namelen - 2] = '\0';
        }
        fprintf(stderr, "Writing %s...\n", filename);

        out = fopen(filename, "wb");
        if(out == NULL) {
            fprintf(stderr, "Couldn't open %s for writing: %s\n", filename, strerror(errno));
            goto error1;
        }

        if(fwrite(blocks[0].data, 1, EDID_PAGE_SIZE, out) < EDID_PAGE_SIZE) {
            fprintf(stderr, "Couldn't write to %s: %s\n", argv[2], strerror(errno));
            goto error2;
        }
      
        fclose(out);
    } else {
        for(i = 0; i < 256; i++) {
            blocks[i].unique = 1;
            if(read_eedid_block(i2cdev, i, blocks[i].data) < 0) {
                fprintf(stderr, "Couldn't read EDID block.\n");
                goto error1;
            }

            for(j = 0; j < i; j++) {
                if(blocks[j].unique == 0) {
                    continue;
                }

                if(memcmp(blocks[i].data, blocks[j].data, EDID_PAGE_SIZE) == 0) {
                    blocks[i].unique = 0;
                    break;
                }
            }

            if(i == j) {
                char filename[namelen + 2]; /* for terminator and 3 digit number */
                memcpy(filename, template, reppos);
                snprintf(&(filename[reppos]), 4, "%03i", i);
                memcpy(&(filename[reppos+3]), &(repchar[2]), namelen - reppos - 2);
                filename[namelen + 1] = '\0';
                fprintf(stderr, "Writing %s...\n", filename);

                out = fopen(filename, "wb");
                if(out == NULL) {
                    fprintf(stderr, "Couldn't open %s for writing: %s\n",
                                    filename, strerror(errno));
                    goto error1;
                }

                if(fwrite(blocks[0].data, 1, EDID_PAGE_SIZE, out) < EDID_PAGE_SIZE) {
                    fprintf(stderr, "Couldn't write to %s: %s\n", argv[2], strerror(errno));
                    goto error2;
                }

                fclose(out);
            } else {
                fprintf(stderr, "Page %i same as %i, not writing.\n", i, j);
            }
        }
    }

    close(i2cdev);
    exit(EXIT_SUCCESS);

error2:
    fclose(out);
error1:
    close(i2cdev);
error0:
    exit(EXIT_FAILURE);
}
