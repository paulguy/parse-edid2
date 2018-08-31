#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "edid-structure.h"
#include "growablechar.h"

void unpackManuID(unsigned char *manuID, const unsigned char *data);

EDID *unpackEDID(unsigned char *data) {
    EDID_Raw *raw = (EDID_Raw *)data;
    EDID *edid;
    int i;

    if(memcmp(raw->magic, EDID_MAGIC, sizeof(EDID_MAGIC)) != 0) {
        fprintf(stderr, "Invalid magic.\n");
        return(NULL);
    }

    edid = malloc(sizeof(EDID));
    if(edid == NULL) {
        fprintf(stderr, "Failed to allocate memory.\n");
        return(NULL);
    }

    unpackManuID(edid->manuID, raw->manuID);

    edid->productCode = raw->productCode;
    edid->serial = raw->serial;
    edid->week = raw->week;
    edid->year = raw->year;
    edid->verMajor = raw->verMajor;
    edid->verMinor = raw->verMinor;

    edid->inputType = raw->attribs & INPUTTYPE_BITS;
    if(edid->inputType == INPUT_DIGITAL) {
        edid->bitDepth = raw->attribs & BITDEPTH_BITS;
        edid->interface = raw->attribs & INTERFACE_BITS;
    } else { /* analog */
        edid->levels = raw->attribs & LEVELS_BITS;
        edid->attribs = raw->attribs & ~LEVELS_BITS;
    }
    
    edid->width = raw->width;
    edid->height = raw->height;
    edid->gamma = raw->gamma;
    edid->features = raw->features;

    /* chromaticity coordinates */
    edid->whitePointX = ((raw->bw_lsb & 0x0C) >> 2) |
                        ((unsigned short)raw->whitex_msb << 2);
    edid->whitePointY = (raw->bw_lsb & 0x03) |
                        ((unsigned short)raw->whitey_msb << 2);
    edid->redPointX = ((raw->rg_lsb & 0xC0) >> 6) |
                      ((unsigned short)raw->redx_msb << 2);
    edid->redPointY = ((raw->rg_lsb & 0x30) >> 4) |
                      ((unsigned short)raw->redy_msb << 2);
    edid->greenPointX = ((raw->rg_lsb & 0x0C) >> 2) |
                        ((unsigned short)raw->greenx_msb << 2);
    edid->greenPointY = (raw->rg_lsb & 0x03) |
                        ((unsigned short)raw->greeny_msb << 2);
    edid->bluePointX = ((raw->bw_lsb & 0xC0) >> 6) |
                       ((unsigned short)raw->bluex_msb << 2);
    edid->bluePointY = ((raw->bw_lsb & 0x30) >> 4) |
                       ((unsigned short)raw->bluey_msb << 2);

    edid->oldModes = ((unsigned int)(raw->timingsBitmap[0]) << 16) |
                     ((unsigned int)(raw->timingsBitmap[1]) << 8) |
                     (unsigned int)(raw->timingsBitmap[2]);

    /* standard timing information */
    for(i = 0; i < 8; i++) {
        edid->resolutionsX[i] = raw->timings[i].xres;
        edid->aspects[i] = raw->timings[i].ar_vf & ASPECT_RATIO_BITS;
        edid->vRefreshes[i] = raw->timings[i].ar_vf & ~ASPECT_RATIO_BITS;
    }

    for(i = 0; i < 4; i++) {
        memcpy(&(edid->descriptors[i]), raw->descs[i], sizeof(EDID_Descriptor));
    }

    edid->extensions = raw->extensions;
    edid->checksum = raw->checksum;

    edid->checksumBad = 0;
    for(i = 0; i < 128; i++) {
        edid->checksumBad += (unsigned int)(data[i]);
    }
    edid->checksumBad &= 0xFF;

    return(edid);
}

#define APPEND_STR(F, ...) \
    if(GC_appendf(gc, F, __VA_ARGS__) < 0) { \
        return(NULL); \
    }
char *parseEDID(EDID *edid) {
    GrowableChar *gc;

    gc = GC_new();
    if(gc == NULL) {
        return(NULL);
    }

    /* no ; after these macros, please */
    APPEND_STR("Manufacturer ID: %s\n", edid->manuID)
    APPEND_STR("Product Code: %04hX\n", edid->productCode)
    APPEND_STR("Serial: %08X\n", edid->serial)
    if(edid->week == 255) {
        APPEND_STR("Model year: %hu\n", (unsigned short)(edid->year) + 1990)
    } else {
        APPEND_STR("Week: %hhu\n", edid->week)
        APPEND_STR("Year: %hu\n", (unsigned short)(edid->year) + 1990)
    }
    APPEND_STR("EDID Version: %hhu.%hhu\n", edid->verMajor, edid->verMinor);

    return(GC_finish(gc));
}
#undef APPEND_STR

void unpackManuID(unsigned char *manuID, const unsigned char *data) {
    manuID[0] = EDID_CONVTABLE[(data[0] & 0x7C) >> 2];
    manuID[1] = EDID_CONVTABLE[((data[0] & 0x03) << 3) | ((data[1] & 0xE0) >> 5)];
    manuID[2] = EDID_CONVTABLE[data[1] & 0x1F];
    manuID[3] = '\0';
}

