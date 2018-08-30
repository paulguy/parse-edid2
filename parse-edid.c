#include <stdio.h>
#include <stdlib.h>


int main(int argc, char **argv) {
    FILE *in;
    unsigned char data[256];
    int edidsize;

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
