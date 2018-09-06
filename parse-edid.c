#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include "edid.h"

int main(int argc, char **argv) {
    FILE *in;
    EDID *edid;
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

    edid = unpackEDID(in);
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
