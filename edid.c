#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "edid-structure.h"
#include "growablechar.h"

#define READ_ARRAY(FILEH, PTR, SIZE) \
    if(fread(PTR, 1, SIZE, FILEH) < SIZE) { \
        fprintf(stderr, "Failed to read " #SIZE " bytes: %s\n", strerror(errno)); \
        return(NULL); \
    }

#define READ_CHAR(FILEH, PTR) \
    READ_ARRAY(FILEH, &(PTR), 1)

#define READ_SHORT(FILEH, PTR) \
    READ_ARRAY(FILEH, &(PTR), 2)

#define READ_INT(FILEH, PTR) \
    READ_ARRAY(FILEH, &(PTR), 4)

#define SKIPOVER(FILEH, AMOUNT) \
    if(fseek(FILEH, AMOUNT, SEEK_CUR) < 0) { \
        fprintf(stderr, "Failed to seek " #AMOUNT " bytes: %s\n", strerror(errno)); \
        return(NULL); \
    }

EDID *unpackEDID(FILE *in) {
    EDID *edid;
    int edidsize;
    unsigned char tbuf[128];
    unsigned short tshort;
    int i, j;

    if(fseek(in, 0, SEEK_END) < 0) {
        fprintf(stderr, "Failed to seek: %s\n", strerror(errno));
        return(NULL);
    }
    edidsize = ftell(in);
    if(edidsize < 0) {
        fprintf(stderr, "Failed to get size: %s\n", strerror(errno));
        return(NULL);
    }
    if(edidsize < 128) {
        fprintf(stderr, "EDID must be at least 128 bytes.\n");
        return(NULL);
    }
    rewind(in);

    READ_ARRAY(in, tbuf, 8)
    if(memcmp(tbuf, EDID_MAGIC, sizeof(EDID_MAGIC)) != 0) {
        fprintf(stderr, "Invalid magic.\n");
        return(NULL);
    }

    edid = malloc(sizeof(EDID));
    if(edid == NULL) {
        fprintf(stderr, "Failed to allocate memory.\n");
        return(NULL);
    }

    READ_ARRAY(in, tbuf, 2)
    unpackManuID(edid->manuID, tbuf);

    READ_SHORT(in, edid->productCode)
    READ_INT(in, edid->serial)
    READ_CHAR(in, edid->week)
    READ_CHAR(in, tbuf[0])
    edid->year = (unsigned short)tbuf[0] + 1990;
    READ_CHAR(in, edid->verMajor)
    READ_CHAR(in, edid->verMinor)

    READ_CHAR(in, tbuf[0])
    edid->inputType = tbuf[0] & INPUTTYPE_BITS;
    if(edid->inputType == INPUT_DIGITAL) {
        edid->bitDepth = tbuf[0] & BITDEPTH_BITS;
        edid->interface = tbuf[0] & INTERFACE_BITS;
    } else { /* analog */
        edid->levels = tbuf[0] & LEVELS_BITS;
        edid->attribs = tbuf[0] & ~LEVELS_BITS;
    }

    READ_CHAR(in, edid->width)
    READ_CHAR(in, edid->height)
    READ_CHAR(in, edid->gamma)
    READ_CHAR(in, tbuf[0])

    edid->features = tbuf[0] & ~DISPLAY_TYPE_BITS;
    if(edid->inputType == INPUT_DIGITAL) {
        edid->dType = tbuf[0] & DISPLAY_TYPE_BITS;
    } else {
        edid->aType = tbuf[0] & DISPLAY_TYPE_BITS;
    }

    /* chromaticity coordinates */
    READ_ARRAY(in, tbuf, 2)
    READ_CHAR(in, edid->redPointX)
    READ_CHAR(in, edid->redPointY)
    READ_CHAR(in, edid->greenPointX)
    READ_CHAR(in, edid->greenPointY)
    READ_CHAR(in, edid->bluePointX)
    READ_CHAR(in, edid->bluePointY)
    READ_CHAR(in, edid->whitePointX)
    READ_CHAR(in, edid->whitePointY)

    edid->redPointX = ((tbuf[0] & 0xC0) >> 6) | (edid->redPointX << 2);
    edid->redPointY = ((tbuf[0] & 0x30) >> 4) | (edid->redPointY << 2);
    edid->greenPointX = ((tbuf[0] & 0x0C) >> 2) | (edid->greenPointX << 2);
    edid->greenPointY = (tbuf[0] & 0x03) | (edid->greenPointY << 2);
    edid->bluePointX = ((tbuf[1] & 0xC0) >> 6) | (edid->bluePointX << 2);
    edid->bluePointY = ((tbuf[1] & 0x30) >> 4) | (edid->bluePointY << 2);
    edid->whitePointX = ((tbuf[1] & 0x0C) >> 2) | (edid->whitePointX << 2);
    edid->whitePointY = (tbuf[1] & 0x03) | (edid->whitePointY << 2);

    READ_ARRAY(in, tbuf, 3)
    edid->oldModes = ((unsigned int)(tbuf[0]) << 16) |
                     ((unsigned int)(tbuf[1]) << 8) |
                     (unsigned int)(tbuf[2]);

    /* standard timing information */
    for(i = 0; i < 8; i++) {
        READ_ARRAY(in, tbuf, 2)
        UNPACK_2BIT_TIMING(edid->resolutionsX[i],
                           edid->aspects[i],
                           edid->vRefreshes[i],
                           tbuf)
    }

    for(i = 0; i < 4; i++) {
        READ_SHORT(in, tshort)
        if(tshort == 0) { /* other */
            SKIPOVER(in, 1)
            READ_CHAR(in, tbuf[0])
            if(tbuf[0] == DESCRIPTOR_NEWER_MODES) {
                EDID_NewerModes *newerModes = &(edid->descriptors[i].newerModes);
                newerModes->type = DESCRIPTOR_NEWER_MODES;

                SKIPOVER(in, 1)
                READ_CHAR(in, newerModes->version)
                READ_ARRAY(in, tbuf, 6)
                newerModes->bitmap = (unsigned long long int)tbuf[0] |
                    ((unsigned long long int)tbuf[1] << 8) |
                    ((unsigned long long int)tbuf[2] << 16) |
                    ((unsigned long long int)tbuf[3] << 24) |
                    ((unsigned long long int)tbuf[4] << 32) |
                    ((unsigned long long int)tbuf[5] << 40);
                READ_ARRAY(in, newerModes->unused, sizeof(newerModes->unused))
            } else if(tbuf[0] == DESCRIPTOR_CVT) {
                EDID_CVT *CVT = &(edid->descriptors[i].CVT);
                CVT->type = DESCRIPTOR_CVT;

                SKIPOVER(in, 1)
                READ_CHAR(in, CVT->version)
                for(j = 0; j < 4; j++) {
                    READ_ARRAY(in, tbuf, 3)
                    CVT->addressableLines[j] = tbuf[0] | 
                        ((unsigned short)(tbuf[1] & 0xF0) << 4);
                    CVT->preferredRate[j] = tbuf[1] &
                        CVT_PREFERRED_RATE_BITS;
                    CVT->aspects[j] = tbuf[2] & CVT_ASPECT_BITS;
                    CVT->rates[j] = tbuf[3] & CVT_RATES_BITS;
                }
            } else if(tbuf[0] == DESCRIPTOR_DCM) {
                EDID_DCM *DCM = &(edid->descriptors[i].DCM);
                DCM->type = DESCRIPTOR_DCM;

                SKIPOVER(in, 1)
                READ_CHAR(in, DCM->version)
                READ_ARRAY(in, tbuf, 12)
                DCM->redA3 = tbuf[0] | ((unsigned short)tbuf[1] << 8);
                DCM->redA2 = tbuf[2] | ((unsigned short)tbuf[3] << 8);
                DCM->greenA3 = tbuf[4] | ((unsigned short)tbuf[5] << 8);
                DCM->greenA2 = tbuf[6] | ((unsigned short)tbuf[7] << 8);
                DCM->blueA3 = tbuf[8] | ((unsigned short)tbuf[9] << 8);
                DCM->blueA2 = tbuf[10] | ((unsigned short)tbuf[11] << 8);
            } else if(tbuf[0] == DESCRIPTOR_TIMINGS) {
                EDID_Timings *timings = &(edid->descriptors[i].timings);
                timings->type = DESCRIPTOR_TIMINGS;

                SKIPOVER(in, 1)
                for(j = 0; j < 6; j++) {
                    READ_ARRAY(in, tbuf, 2)
                    UNPACK_2BIT_TIMING(timings->resolutionsX[j],
                                       timings->aspects[j],
                                       timings->vRefreshes[j],
                                       tbuf)
                }
                SKIPOVER(in, 1)
            } else if(tbuf[0] == DESCRIPTOR_WHITEP) {
                EDID_WhitePoints *whitePoints = &(edid->descriptors[i].whitePoints);
                whitePoints->type = DESCRIPTOR_WHITEP;

                SKIPOVER(in, 1)
                for(j = 0; j < 2; j++) {
                    READ_ARRAY(in, tbuf, 5)
                    whitePoints->indices[j] = tbuf[0];
                    whitePoints->xes[j] = 
                        ((tbuf[1] & 0x0C) >> 2) | ((unsigned short)tbuf[2] << 2);
                    whitePoints->ys[j] = 
                        (tbuf[1] & 0x03) | ((unsigned short)tbuf[3] << 2);
                    whitePoints->gammas[j] = tbuf[4];
                }
                READ_ARRAY(in, whitePoints->unused, sizeof(whitePoints->unused))
            } else if(tbuf[0] == DESCRIPTOR_NAME) {
                EDID_Text *text = &(edid->descriptors[i].text);
                text->type = DESCRIPTOR_NAME;

                SKIPOVER(in, 1)
                READ_ARRAY(in, text->text, sizeof(text->text))
            } else if(tbuf[0] == DESCRIPTOR_LIMITS) {
                EDID_RangeLimits *rangeLimits = &(edid->descriptors[i].rangeLimits);
                rangeLimits->type = DESCRIPTOR_LIMITS;

                READ_ARRAY(in, tbuf, 6)
                rangeLimits->vMinRate = tbuf[1] |
                    ((unsigned short)(tbuf[0] & 0x1) << 8);
                rangeLimits->vMaxRate = tbuf[2] |
                    ((unsigned short)(tbuf[0] & 0x2) << 7);
                rangeLimits->hMinRate = tbuf[3] |
                    ((unsigned short)(tbuf[0] & 0x4) << 6);
                rangeLimits->hMaxRate = tbuf[4] |
                    ((unsigned short)(tbuf[0] & 0x8) << 5);
                rangeLimits->pixelClock = (unsigned short)tbuf[5] * 10;

                READ_CHAR(in, rangeLimits->extLimitsType)
                if(rangeLimits->extLimitsType == EXT_LIMITS_GTF) {
                    SKIPOVER(in, 1)
                    READ_CHAR(in, rangeLimits->GTFStart)
                    READ_CHAR(in, rangeLimits->GTFC)
                    READ_SHORT(in, rangeLimits->GTFM)
                    READ_CHAR(in, rangeLimits->GTFK)
                    READ_CHAR(in, rangeLimits->GTFJ)
                } else if(rangeLimits->extLimitsType == EXT_LIMITS_CVT) {
                    READ_CHAR(in, rangeLimits->CVTVersion)
                    READ_ARRAY(in, tbuf, 2)
                    rangeLimits->CVTExtraPrecision = (tbuf[0] & 0xFC) >> 2;
                    rangeLimits->CVTMaxWidth = tbuf[1] | 
                        ((unsigned short)(tbuf[0] & 0x03) << 8);
                    READ_CHAR(in, rangeLimits->CVTAspects)
                    READ_CHAR(in, rangeLimits->CVTPrefs)
                    READ_CHAR(in, rangeLimits->CVTScaling)
                    READ_CHAR(in, rangeLimits->CVTPrefRefresh)
                } else {
                    SKIPOVER(in, 7)
                }
            } else if(tbuf[0] == DESCRIPTOR_TEXT) {
                EDID_Text *text = &(edid->descriptors[i].text);
                text->type = DESCRIPTOR_TEXT;

                SKIPOVER(in, 1)
                READ_ARRAY(in, text->text, sizeof(text->text))
            } else if(tbuf[0] == DESCRIPTOR_SERIAL) {
                EDID_Text *text = &(edid->descriptors[i].text);
                text->type = DESCRIPTOR_SERIAL;

                SKIPOVER(in, 1)
                READ_ARRAY(in, text->text, sizeof(text->text))
            } else {
                EDID_Unknown *unknown = &(edid->descriptors[i].unknown);
                unknown->type = DESCRIPTOR_UNKNOWN;

                SKIPOVER(in, -4)
                READ_ARRAY(in, unknown->data, sizeof(unknown->data))
            }
        } else { /* DTD */
            EDID_DTD *DTD = &(edid->descriptors[i].DTD);
            DTD->type = DESCRIPTOR_DTD;

            DTD->clock = tshort;
            READ_ARRAY(in, tbuf, 3)
            DTD->hActive = ((unsigned short)(tbuf[2] & 0xF0) << 4) |
                           (unsigned short)(tbuf[0]);
            DTD->hBlanking = ((unsigned short)(tbuf[2] & 0x0F) << 8) |
                             (unsigned short)(tbuf[1]);
            READ_ARRAY(in, tbuf, 3)
            DTD->vActive = ((unsigned short)(tbuf[2] & 0xF0) << 4) |
                           (unsigned short)(tbuf[0]);
            DTD->vBlanking = ((unsigned short)(tbuf[2] & 0x0F) << 8) |
                             (unsigned short)(tbuf[1]);
            READ_ARRAY(in, tbuf, 4)
            DTD->hFrontPorch = ((unsigned short)(tbuf[3] & 0xC0) << 2) |
                               (unsigned short)(tbuf[0]);
            DTD->hSyncPulse = ((unsigned short)(tbuf[3] & 0x30) << 4) |
                              (unsigned short)(tbuf[1]);
            DTD->vFrontPorch = (tbuf[2] & 0x0C << 2) |
                               (tbuf[3] & 0xF0 >> 4);
            DTD->vSyncPulse = (tbuf[2] & 0x03 << 4) |
                              (tbuf[3] & 0x0F);
            READ_ARRAY(in, tbuf, 3)
            DTD->width = ((unsigned short)(tbuf[2] & 0xF0) << 4) |
                         (unsigned short)(tbuf[1]);
            DTD->height = ((unsigned short)(tbuf[2] & 0x0F) << 8) |
                          (unsigned short)(tbuf[1]);
            READ_CHAR(in, DTD->hBorder)
            READ_CHAR(in, DTD->vBorder)
            READ_CHAR(in, DTD->features)
        }
    }

    READ_CHAR(in, edid->extensions)
    READ_CHAR(in, edid->checksum)

    rewind(in);
    READ_ARRAY(in, tbuf, 128)

    edid->checksumBad = 0;
    for(i = 0; i < 128; i++) {
        edid->checksumBad += (unsigned int)(tbuf[i]);
    }
    edid->checksumBad &= 0xFF;

    return(edid);
}

#define APPEND_STR(F) \
    if(GC_appendf(gc, F) < 0) { \
        GC_free(gc); \
        return(NULL); \
    }

#define APPEND_STRF(F, ...) \
    if(GC_appendf(gc, F, __VA_ARGS__) < 0) { \
        GC_free(gc); \
        return(NULL); \
    }

#define ADD_ONE_OR_OTHER(MSG, EXPR, ONE, OTHER) \
    APPEND_STR(MSG) \
    if(EXPR) { \
        APPEND_STR(ONE) \
    } else { \
        APPEND_STR(OTHER) \
    }

#define ADD_YES_OR_NO(MSG, EXPR) \
    ADD_ONE_OR_OTHER(MSG, EXPR, "Yes\n", "No\n")

#define ADD_MODE(XRES, YRES, HZ) \
    ADD_YES_OR_NO(#XRES "x" #YRES "@" #HZ ": ", \
                  edid->oldModes & MODE_ ## XRES ## _ ## YRES ## _ ## HZ )

#define ADD_INTERLACED_MODE(XRES, YRES, HZ) \
    ADD_YES_OR_NO(#XRES "x" #YRES "@" #HZ " Interlaced: ", \
                  edid->oldModes & MODE_ ## XRES ## _ ## YRES ## _ ## HZ ## I )

#define ADD_NEWER_MODE(XRES, YRES, HZ) \
    ADD_YES_OR_NO(#XRES "x" #YRES "@" #HZ ": ", \
                  newerModes & NMODE_ ## XRES ## _ ## YRES ## _ ## HZ )

#define ADD_NEWER_REDUCED_MODE(XRES, YRES, HZ) \
    ADD_YES_OR_NO(#XRES "x" #YRES "@" #HZ " RB: ", \
                  newerModes & NMODE_ ## XRES ## _ ## YRES ## _ ## HZ ## R )

#define ADD_2BYTE_MODE(RESX, ASPECTR, VREF) \
    if(isDefined2ByteMode(RESX, ASPECTR, VREF)) { \
        APPEND_STRF("%hux", RESX) \
        if(ASPECTR == ASPECT_16_10_1_1) { \
            if(versionCompare(edid->verMajor, edid->verMinor, 1, 3) < 0) { /* 1:1 */ \
                APPEND_STRF("%hu", RESX) \
            } else { \
                APPEND_STRF("%hu", RESX / 16 * 10) \
            } \
        } else if(ASPECTR == ASPECT_4_3) { \
            APPEND_STRF("%hu", RESX / 4 * 3) \
        } else if(ASPECTR == ASPECT_5_4) { \
            APPEND_STRF("%hu", RESX / 5 * 4) \
        } else if(ASPECTR == ASPECT_16_9) { \
            APPEND_STRF("%hu", RESX / 16 * 9) \
        } \
        APPEND_STRF("@%hhu\n", VREF); \
    } else { \
        APPEND_STR("Unused Mode Field\n") \
    }

#define APPEND_HEX(DATA, SIZE) \
    for(j = 0; j < SIZE; j++) { \
        if(j % 16 == 0) { \
            APPEND_STRF("\n%04i: ", j) \
        } \
        if(j % 16 == 8) { \
            APPEND_STRF("-%02X", DATA[j]) \
        } else { \
            APPEND_STRF(" %02X", DATA[j]) \
        } \
    } \
    APPEND_STR("\n")

char *parseEDID(EDID *edid) {
    GrowableChar *gc;
    unsigned int i, j;

    gc = GC_new();
    if(gc == NULL) {
        return(NULL);
    }

    /* no ; after these macros, please */
    APPEND_STR("*** EDID Header ***\n")
    APPEND_STRF("Manufacturer ID: %s\n", edid->manuID)
    APPEND_STRF("Product Code: %04hX\n", edid->productCode)
    APPEND_STRF("Serial: %08X\n", edid->serial)
    if(edid->week == 255) {
        APPEND_STRF("Model year: %hu\n", edid->year)
    } else {
        APPEND_STRF("Week: %hhu\n", edid->week)
        APPEND_STRF("Year: %hu\n", edid->year)
    }
    APPEND_STRF("EDID Version: %hhu.%hhu\n", edid->verMajor, edid->verMinor)

    APPEND_STR("\n*** Basic Display Parameters ***\n")
    APPEND_STRF("Input type: %s\n", inputTypeToStr(edid->inputType))
    if(edid->inputType == INPUT_DIGITAL) {
        APPEND_STRF("Digital Bit Depth: %s\n", bitDepthToStr(edid->bitDepth))
        APPEND_STRF("Digital Interface: %s\n", interfaceToStr(edid->interface))
    } else if(edid->inputType == INPUT_ANALOG) {
        APPEND_STRF("Analog White/Sync Voltages: %s\n", voltsToStr(edid->levels))
        ADD_YES_OR_NO("Black-to-Black Setup Expected: ",
                      edid->attribs & BLACK_TO_BLACK_BIT)
        ADD_YES_OR_NO("Separate Sync Supported: ",
                      edid->attribs & SEPARATE_SYNC_BIT)
        ADD_YES_OR_NO("Composite HSync Supported: ",
                      edid->attribs & COMPOSITE_SYNC_BIT)
        ADD_YES_OR_NO("Sync On Green Supported: ",
                      edid->attribs & SYNC_ON_GREEN_BIT)
        ADD_YES_OR_NO("VSync pulse must be serrated when composite or sync-on-green is used: ",
                      edid->attribs & SERRATED_VSYNC_BIT)
    }
    if(edid->width == 0) {
        if(edid->height == 0) {
            APPEND_STR("Screen Size or Aspect Ratio are undefined.\n")
        } else {
            APPEND_STRF("Aspect Ratio: %hhu.%hhu:1\n",
                        edid->height / 100 + 1,
                        edid->height % 100)
        }
    } else if(edid->height == 0) {
        APPEND_STRF("Aspect Ratio: %.2f:1\n",
                   100.0 / (float)(edid->width) - 99.0)
    } else {
        APPEND_STRF("Width: %hhu cm\n", edid->width)
        APPEND_STRF("Height: %hhu cm\n", edid->height)
    }
    if(edid->gamma == 255) {
        APPEND_STR("Gamma defined in a descriptor block.\n")
    } else {
        APPEND_STRF("Gamma: %hhu.%hhu\n",
                    edid->gamma / 100 + 1,
                    edid->gamma % 100)
    }

    ADD_YES_OR_NO("DPMS Standby Supported: ",
                  edid->features & STANDBY_BIT)
    ADD_YES_OR_NO("DPMS Suspend Supported: ",
                  edid->features & SUSPEND_BIT)
    ADD_YES_OR_NO("DPMS Active-off Supported: ",
                  edid->features & ACTIVEOFF_BIT)
    if(edid->inputType == INPUT_DIGITAL) {
        APPEND_STRF("Digital Display Type: %s\n", dTypeToStr(edid->dType))
    } else if(edid->inputType == INPUT_ANALOG) {
        APPEND_STRF("Analog Display Type: %s\n", aTypeToStr(edid->aType))
    }        
    ADD_YES_OR_NO("sRGB Color Space: ", edid->features & SRGB_BIT)
    if(versionCompare(edid->verMajor, edid->verMinor, 1, 3) < 0) {
        ADD_YES_OR_NO("Preferred Timing Mode is in First Destailed Timing Descriptor: ",
                      edid->features & PREFERRED_BIT)
    } else {
        ADD_YES_OR_NO("Preferred Timing Mode is Native: ",
                      edid->features & PREFERRED_BIT)
    }
    ADD_YES_OR_NO("Default GTF Supported: ",
                  edid->features & GTF_BIT)

    APPEND_STR("\n*** Chromaticity Coordinates ***\n")
    APPEND_STRF("Red Point (x, y): (%hu, %hu)\n", edid->redPointX, edid->redPointY)
    APPEND_STRF("Green Point (x, y): (%hu, %hu)\n", edid->greenPointX, edid->greenPointY)
    APPEND_STRF("Blue Point (x, y): (%hu, %hu)\n", edid->bluePointX, edid->bluePointY)
    APPEND_STRF("White Point (x, y): (%hu, %hu)\n", edid->whitePointX, edid->whitePointY)

    APPEND_STR("\n*** Established Timings ***\n")
    ADD_MODE(720, 400, 70)
    ADD_MODE(720, 400, 88)
    ADD_MODE(640, 480, 60)
    ADD_MODE(640, 480, 67)
    ADD_MODE(640, 480, 72)
    ADD_MODE(640, 480, 75)
    ADD_MODE(800, 600, 56)
    ADD_MODE(800, 600, 60)
    ADD_MODE(800, 600, 72)
    ADD_MODE(800, 600, 75)
    ADD_MODE(832, 624, 75)
    ADD_INTERLACED_MODE(1024, 768, 87)
    ADD_MODE(1024, 768, 60)
    ADD_MODE(1024, 768, 72)
    ADD_MODE(1024, 768, 75)
    ADD_MODE(1280, 1024, 75)
    ADD_MODE(1152, 870, 75)
    if(edid->oldModes & MODE_MANUFACTURER) {
        APPEND_STR("Manufacturer-Specific Modes Set.\n");
    }

    APPEND_STR("\n*** Standard Timing Information ***\n")
    for(i = 0; i < 8; i++) {
        ADD_2BYTE_MODE(edid->resolutionsX[i], edid->aspects[i], edid->vRefreshes[i])
    }

    APPEND_STR("\n*** Descriptor Blocks ***")
    for(i = 0; i < 4; i++) {
        APPEND_STR("\n")
        /* the ;s are needed after the cases because reasons */
        switch(edid->descriptors[i].type) {
        case DESCRIPTOR_DTD: ;
            EDID_DTD *DTD = &(edid->descriptors[i].DTD);

            APPEND_STRF("%i: Detailed Timing Descriptor\n", i)
            APPEND_STRF("Pixel Clock: %hu.%hu Mhz\n", DTD->clock / 100, DTD->clock % 100)
            APPEND_STRF("Horizontal Active Pixels: %hu\n", DTD->hActive)
            APPEND_STRF("Horizontal Blanking Pixels: %hu\n", DTD->hBlanking)
            APPEND_STRF("Vertical Active Lines: %hu\n", DTD->vActive)
            APPEND_STRF("Vertical Blanking Lines: %hu\n", DTD->vBlanking)
            APPEND_STRF("Horizontal Front Porch Pixels: %hu\n", DTD->hFrontPorch)
            APPEND_STRF("Horizontal Sync Pulse Pixels: %hu\n", DTD->hSyncPulse)
            APPEND_STRF("Vertical Front Porch Lines: %hhu\n", DTD->vFrontPorch)
            APPEND_STRF("Vertical Sync Pulse Lines: %hhu\n", DTD->vSyncPulse)
            APPEND_STRF("Width: %hu mm\n", DTD->width)
            APPEND_STRF("Height: %hu mm\n", DTD->height)
            APPEND_STRF("Horizontal Border Pixels: %hhu per side\n", DTD->hBorder)
            APPEND_STRF("Vertical Border Lines: %hhu per side\n", DTD->vBorder)
            ADD_YES_OR_NO("Interlaced: ",
                          DTD->features & INTERLACED_BIT)
            APPEND_STRF("Stereo Mode: %s\n", stereoModeToStr(DTD->features & STEREO_BITS))
            if(DTD->features & DIGITAL_SYNC_BIT) {
                if(DTD->features & SYNC_TYPE_BIT) {
                    APPEND_STR("Sync Type: Digital Separate Sync\n")
                    ADD_YES_OR_NO("Sync Serration (HSync During VSync): ",
                                  DTD->features & DTD_SERRATED_SYNC_BIT)
                    ADD_ONE_OR_OTHER("Horizontal Sync Polarity: ",
                                     DTD->features & HSYNC_POLARITY_BIT,
                                     "Positive\n", "Negative\n")
                } else {
                    APPEND_STR("Sync Type: Digital Composite Sync\n")
                    ADD_ONE_OR_OTHER("Vertical Sync Polarity: ",
                                     DTD->features & VSYNC_POLARITY_BIT,
                                     "Positive\n", "Negative\n")
                }
            } else {
                APPEND_STR("Sync Type: Analog Sync\n")
                ADD_ONE_OR_OTHER("Analog Sync Type: ",
                                 DTD->features & SYNC_TYPE_BIT,
                                 "Bipolar Analog Composite\n",
                                 "Analog Composite\n")
                ADD_YES_OR_NO("Sync Serration (HSync During VSync): ",
                              DTD->features & DTD_SERRATED_SYNC_BIT)
            }
            break;
        case DESCRIPTOR_NEWER_MODES: ;
            unsigned long long int newerModes = edid->descriptors[i].newerModes.bitmap;

            APPEND_STRF("%i: Additional Standard Timings\n", i)
            ADD_NEWER_MODE(640, 350, 85)
            ADD_NEWER_MODE(640, 400, 85)
            ADD_NEWER_MODE(720, 400, 85)
            ADD_NEWER_MODE(640, 480, 85)
            ADD_NEWER_MODE(848, 480, 60)
            ADD_NEWER_MODE(800, 600, 85)
            ADD_NEWER_MODE(1024, 768, 85)
            ADD_NEWER_MODE(1152, 864, 85)
            ADD_NEWER_REDUCED_MODE(1280, 768, 60)
            ADD_NEWER_MODE(1280, 768, 60)
            ADD_NEWER_MODE(1280, 768, 75)
            ADD_NEWER_MODE(1280, 768, 85)
            ADD_NEWER_MODE(1280, 960, 60)
            ADD_NEWER_MODE(1280, 960, 85)
            ADD_NEWER_MODE(1280, 1024, 60)
            ADD_NEWER_MODE(1280, 1024, 85)
            ADD_NEWER_REDUCED_MODE(1366, 768, 60)
            ADD_NEWER_MODE(1366, 768, 60)
            ADD_NEWER_REDUCED_MODE(1440, 900, 60)
            ADD_NEWER_MODE(1440, 900, 75)
            ADD_NEWER_MODE(1440, 900, 85)
            ADD_NEWER_REDUCED_MODE(1440, 1050, 60)
            ADD_NEWER_MODE(1440, 1050, 60)
            ADD_NEWER_MODE(1440, 1050, 75)
            ADD_NEWER_MODE(1440, 1050, 85)
            ADD_NEWER_REDUCED_MODE(1680, 1050, 60)
            ADD_NEWER_MODE(1680, 1050, 60)
            ADD_NEWER_MODE(1680, 1050, 75)
            ADD_NEWER_MODE(1680, 1050, 85)
            ADD_NEWER_MODE(1600, 1200, 60)
            ADD_NEWER_MODE(1600, 1200, 65)
            ADD_NEWER_MODE(1600, 1200, 70)
            ADD_NEWER_MODE(1600, 1200, 75)
            ADD_NEWER_MODE(1600, 1200, 85)
            ADD_NEWER_MODE(1792, 1344, 60)
            ADD_NEWER_MODE(1792, 1344, 75)
            ADD_NEWER_MODE(1856, 1392, 60)
            ADD_NEWER_MODE(1856, 1392, 75)
            ADD_NEWER_REDUCED_MODE(1920, 1200, 60)
            ADD_NEWER_MODE(1920, 1200, 60)
            ADD_NEWER_MODE(1920, 1200, 75)
            ADD_NEWER_MODE(1920, 1200, 85)
            ADD_NEWER_MODE(1920, 1440, 60)
            ADD_NEWER_MODE(1920, 1440, 75)
            break;
        case DESCRIPTOR_CVT: ;
            EDID_CVT *CVT = &(edid->descriptors[i].CVT);

            APPEND_STRF("%i: CVT 3-Byte Timing Codes\n", i)
            APPEND_STRF("Version: %hhu\n", CVT->version)
            for(j = 0; j < 4; j++) {
                APPEND_STRF("Timing Descriptor %i\n", i)
                APPEND_STRF("Addressable Lines: %hu\n", CVT->addressableLines[j])
                APPEND_STRF("Preferred Rate: %s\n", preferredRateToStr(CVT->preferredRate[j]))
                APPEND_STRF("Aspect Ratio: %s\n", CVTAspectToStr(CVT->aspects[j]))
                if(CVT->aspects[j] == CVT_ASPECT_16_10) {
                    APPEND_STRF("Calculated Resolution: %ux%hu",
                                (unsigned int)CVT->addressableLines[j] * 16 / 10,
                                CVT->addressableLines[j])
                } else if(CVT->aspects[j] == CVT_ASPECT_4_3) {
                    APPEND_STRF("Calculated Resolution: %ux%hu",
                                (unsigned int)CVT->addressableLines[j] * 4 / 3,
                                CVT->addressableLines[j])
                } else if(CVT->aspects[j] == CVT_ASPECT_5_4) {
                    APPEND_STRF("Calculated Resolution: %ux%hu",
                                (unsigned int)CVT->addressableLines[j] * 5 / 4,
                                CVT->addressableLines[j])
                } else if(CVT->aspects[j] == CVT_ASPECT_16_9) {
                    APPEND_STRF("Calculated Resolution: %ux%hu",
                                (unsigned int)CVT->addressableLines[j] * 16 / 9,
                                CVT->addressableLines[j])
                }
            }
            break;
        case DESCRIPTOR_DCM: ;
            EDID_DCM *DCM = &(edid->descriptors[i].DCM);

            APPEND_STRF("%i: Display Color Management\n", i)
            APPEND_STRF("Version: %hhu\n", DCM->version)
            APPEND_STRF("Red A3, A2: %hu, %hu\n", DCM->redA3, DCM->redA2)
            APPEND_STRF("Green A3, A2: %hu, %hu\n", DCM->greenA3, DCM->greenA2)
            APPEND_STRF("Blue A3, A2: %hu, %hu\n", DCM->blueA3, DCM->blueA2)
            break;
        case DESCRIPTOR_TIMINGS: ;
            EDID_Timings *timings = &(edid->descriptors[i].timings);

            APPEND_STRF("%i: Additional Standard Timings\n", i)
            for(j = 0; j < 6; j++) {
                ADD_2BYTE_MODE(timings->resolutionsX[j],
                               timings->aspects[j],
                               timings->vRefreshes[j])
            }
            break;
        case DESCRIPTOR_WHITEP: ;
            EDID_WhitePoints *whitePoints = &(edid->descriptors[i].whitePoints);

            APPEND_STRF("%i: Additional White Points\n", i)
            for(j = 0; j < 2; j++) {
                APPEND_STRF("Index: %hhu\n", whitePoints->indices[j])
                APPEND_STRF("Coordinates (x, y): (%hu, %hu)\n",
                    whitePoints->xes[j],
                    whitePoints->ys[j])
                APPEND_STRF("Gamma: %hhu\n", whitePoints->gammas[j])
            }
            APPEND_STRF("Unused bytes: %hhu %hhu %hhu\n",
                whitePoints->unused[0],
                whitePoints->unused[1],
                whitePoints->unused[2])
            break;
        case DESCRIPTOR_NAME: ;
            EDID_Text *nameText = &(edid->descriptors[i].text);

            APPEND_STRF("%i: Product Name: %s\n", i, nameText->text)
            break;
        case DESCRIPTOR_LIMITS: ;
            EDID_RangeLimits *rangeLimits = &(edid->descriptors[i].rangeLimits);

            APPEND_STRF("%i: Display Range Limits\n", i)
            APPEND_STRF("Minimum Vertical Field Rate: %hu Hz\n", rangeLimits->vMinRate)
            APPEND_STRF("Maximum Vertical Field Rate: %hu Hz\n", rangeLimits->vMaxRate)
            APPEND_STRF("Minimum Horizontal Line Rate: %hu kHz\n", rangeLimits->hMinRate)
            APPEND_STRF("Minimum Horizontal Line Rate: %hu kHz\n", rangeLimits->hMaxRate)
            APPEND_STRF("Maximum Pixel Clock Rate: %hu MHz\n", rangeLimits->pixelClock)
            APPEND_STRF("Extended Timing Info Type: %s\n",
                extLimitsTypeToStr(rangeLimits->extLimitsType))
            if(rangeLimits->extLimitsType == EXT_LIMITS_GTF) {
                APPEND_STRF("GTF Start Frequency: %hu\n",
                    (unsigned short)rangeLimits->GTFStart * 2)
                APPEND_STRF("GTF C: %hhu.%hhu\n",
                    rangeLimits->GTFC / 2,
                    rangeLimits->GTFC % 2 ? 5 : 0)
                APPEND_STRF("GTF M: %hu\n", rangeLimits->GTFM)
                APPEND_STRF("GTF K: %hhu\n", rangeLimits->GTFK)
                APPEND_STRF("GTF J: %hhu.%hhu\n",
                    rangeLimits->GTFJ / 2,
                    rangeLimits->GTFJ % 2 ? 5 : 0)
            } else if(rangeLimits->extLimitsType == EXT_LIMITS_CVT) {
                APPEND_STRF("CVT Version: %02hhX\n", rangeLimits->CVTVersion)
                APPEND_STRF("CVT Extra Clock Precision: Maximum Pixel Clock Rate - %hhu.%hhu\n",
                    rangeLimits->CVTExtraPrecision / 4,
                    rangeLimits->CVTExtraPrecision % 4 * 25)
                APPEND_STR("Maximum Active Pixels Per Line: ")
                if(rangeLimits->CVTMaxWidth == 0) {
                    APPEND_STR("No Limit\n")
                } else {
                    APPEND_STRF("%hu\n", rangeLimits->CVTMaxWidth)
                }
                ADD_YES_OR_NO("Aspect 4:3 Support: ",
                    rangeLimits->CVTAspects & CVT_ASPECT_4_3_BIT)
                ADD_YES_OR_NO("Aspect 16:9 Support: ",
                    rangeLimits->CVTAspects & CVT_ASPECT_16_9_BIT)
                ADD_YES_OR_NO("Aspect 16:10 Support: ",
                    rangeLimits->CVTAspects & CVT_ASPECT_16_10_BIT)
                ADD_YES_OR_NO("Aspect 5:4 Support: ",
                    rangeLimits->CVTAspects & CVT_ASPECT_5_4_BIT)
                ADD_YES_OR_NO("Aspect 15:9 Support: ",
                    rangeLimits->CVTAspects & CVT_ASPECT_15_9_BIT)
                APPEND_STRF("Preferred Aspect Ratio: %s\n",
                    CVTLimitPrefAspectToStr(rangeLimits->CVTPrefs & CVT_PREF_ASPECT_BITS))
                ADD_YES_OR_NO("CVT Reduced Blanking Support: ",
                    rangeLimits->CVTPrefs & CVT_RB_BIT)
                ADD_YES_OR_NO("CVT Standard Blanking Support: ",
                    rangeLimits->CVTPrefs & CVT_STANDARD_BIT)
                ADD_YES_OR_NO("Horizontal Shrink Support: ",
                    rangeLimits->CVTScaling & CVT_SCALE_XSHRINK_BIT)
                ADD_YES_OR_NO("Horizontal Stretch Support: ",
                    rangeLimits->CVTScaling & CVT_SCALE_XGROW_BIT)
                ADD_YES_OR_NO("Vertical Shrink Support: ",
                    rangeLimits->CVTScaling & CVT_SCALE_XSHRINK_BIT)
                ADD_YES_OR_NO("Vertical Stretch Support: ",
                    rangeLimits->CVTScaling & CVT_SCALE_XGROW_BIT)
                APPEND_STRF("Preferred Vertical Refresh Rate: %hhu Hz\n",
                    rangeLimits->CVTPrefRefresh)
            }
            break;
        case DESCRIPTOR_TEXT: ;
            EDID_Text *text = &(edid->descriptors[i].text);

            APPEND_STRF("%i: Text: %s\n", i, text->text)
            break;
        case DESCRIPTOR_SERIAL: ;
            EDID_Text *serialText = &(edid->descriptors[i].text);

            APPEND_STRF("%i: Serial: %s\n", i, serialText->text)
            break;
        default: ; /* Unknown */
            EDID_Unknown *unknown = &(edid->descriptors[i].unknown);

            APPEND_STRF("%i: Unknown\n", i)
            APPEND_HEX(unknown->data, sizeof(unknown->data))
        }
    }

    APPEND_STRF("\nExtensions: %hhu\n", edid->extensions)
    APPEND_STRF("Checksum: %hhu ", edid->checksum)
    if(edid->checksumBad) {
        APPEND_STR("Bad\n")
    } else {
        APPEND_STR("Good\n")
    }

    return(GC_finish(gc));
}
