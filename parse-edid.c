#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include "edid.h"

int main(int argc, char **argv) {
    EDID *edid;
    FILE *in;
    unsigned char *data;
    int edidsize, readsize;
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

    if(fseek(in, 0, SEEK_END) < 0) {
        fprintf(stderr, "Failed to seek: %s\n", strerror(errno));
        fclose(in);
        exit(EXIT_FAILURE);
    }
    edidsize = ftell(in);
    if(edidsize < 0) {
        fprintf(stderr, "Failed to get size: %s\n", strerror(errno));
        fclose(in);
        exit(EXIT_FAILURE);
    }
    if(edidsize < 128) {
        fprintf(stderr, "EDID must be at least 128 bytes.\n");
        fclose(in);
        exit(EXIT_FAILURE);
    }
    rewind(in);

    data = malloc(edidsize);
    if(data == NULL) {
        fprintf(stderr, "Failed to allocate memory.\n");
        fclose(in);
        exit(EXIT_FAILURE);
    }

    readsize = fread(data, 1, edidsize, in);
    fclose(in);

    if(readsize < edidsize) {
        fprintf(stderr, "Short read.\n");
        exit(EXIT_FAILURE);
    }

    edid = unpackEDID(data);
    free(data);
    if(edid == NULL) {
        fprintf(stderr, "Failed to unpack raw EDID.\n");
        exit(EXIT_FAILURE);
    }

    parsed = parseEDID(edid);
    free(edid);
    if(parsed == NULL) {
        fprintf(stderr, "Failed to parse EDID structure.\n");
        exit(EXIT_FAILURE);
    }

    fputs(parsed, stdout);

    free(parsed);

    return(0);
}
