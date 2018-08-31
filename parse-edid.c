#include <stdio.h>
#include <stdlib.h>

#include "edid.h"

int main(int argc, char **argv) {
    EDID *edid;
    FILE *in;
    unsigned char data[256];
    int edidsize;
    char *parsed;

    if(argc < 2) {
        fprintf(stderr, "USAGE: parse-edid <edid file>\n");
        exit(EXIT_FAILURE);
    }

    in = fopen(argv[1], "rb");
    if(in == NULL) {
        fprintf(stderr, "Failed to open %s.\n", argv[1]);
        exit(EXIT_FAILURE);
    }

    edidsize = fread(data, 1, 256, in);
    fclose(in);

    if(edidsize < 128) {
        fprintf(stderr, "EDID must be at least 128 bytes.\n");
        exit(EXIT_FAILURE);
    }

    edid = unpackEDID(data);
    if(edid == NULL) {
        fprintf(stderr, "Failed to unpack raw EDID.\n");
        exit(EXIT_FAILURE);
    }

    parsed = parseEDID(edid);
    if(parsed == NULL) {
        fprintf(stderr, "Failed to parse EDID structure.\n");
        free(edid);
        exit(EXIT_FAILURE);
    }

    fputs(parsed, stdout);

    free(parsed);
    free(edid);

    return(0);
}
