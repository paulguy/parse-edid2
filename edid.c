#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "edid-structure.h"
#include "growablechar.h"

EDID *unpackEDID(unsigned char *data) {
    EDID_Raw *raw = (EDID_Raw *)data;
    EDID *edid;
    EDID_RawDescriptor *d;
    int i, j;

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
    edid->year = (unsigned short)(raw->year) + 1990;
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
    edid->features = raw->features & ~DISPLAY_TYPE_BITS;
    if(edid->inputType == INPUT_DIGITAL) {
        edid->dType = raw->features & DISPLAY_TYPE_BITS;
    } else {
        edid->aType = raw->features & DISPLAY_TYPE_BITS;
    }

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
        UNPACK_2BIT_TIMING(edid->resolutionsX[i],
                           edid->aspects[i],
                           edid->vRefreshes[i],
                           raw->timings[i])
    }

    for(i = 0; i < 4; i++) {
        d = (EDID_RawDescriptor *)raw->descs[i];
        if(d->DTDOrOther == 0) { /* other */
            if(d->type == DESCRIPTOR_NEWER_MODES) {
                EDID_RawNewerModes *rNewerModes = (EDID_RawNewerModes *)raw->descs[i];
                EDID_NewerModes *newerModes = &(edid->descriptors[i].newerModes);
                newerModes->type = DESCRIPTOR_NEWER_MODES;

                newerModes->version = rNewerModes->version;
                newerModes->bitmap = (unsigned long long int)rNewerModes->bitmaps[5] |
                    ((unsigned long long int)rNewerModes->bitmaps[4] << 8) |
                    ((unsigned long long int)rNewerModes->bitmaps[3] << 16) |
                    ((unsigned long long int)rNewerModes->bitmaps[2] << 24) |
                    ((unsigned long long int)rNewerModes->bitmaps[1] << 32) |
                    ((unsigned long long int)rNewerModes->bitmaps[0] << 40);
            } else if(d->type == DESCRIPTOR_CVT) {
                EDID_RawCVT *rCVT = (EDID_RawCVT *)raw->descs[i];
                EDID_CVT *CVT = &(edid->descriptors[i].CVT);
                CVT->type = DESCRIPTOR_CVT;

                CVT->version = rCVT->version;
                for(j = 0; j < 4; j++) {
                    CVT->addressableLines[j] = rCVT->timings[j].addrLines_lsb | 
                        ((unsigned short)(rCVT->timings[j].lines_msb_pvr & 0xF0) << 4);
                    CVT->preferredRate[j] = rCVT->timings[j].lines_msb_pvr &
                        CVT_PREFERRED_RATE_BITS;
                    CVT->aspects[j] = rCVT->timings[j].ar_vr & CVT_ASPECT_BITS;
                    CVT->rates[j] = rCVT->timings[j].ar_vr & CVT_RATES_BITS;
                }
            } else if(d->type == DESCRIPTOR_DCM) {
                EDID_RawDCM *rDCM = (EDID_RawDCM *)raw->descs[i];
                EDID_DCM *DCM = &(edid->descriptors[i].DCM);
                DCM->type = DESCRIPTOR_DCM;

                DCM->version = rDCM->version;
                DCM->redA3 = rDCM->redA3_lsb |
                    ((unsigned short)rDCM->redA3_msb << 8);
                DCM->redA2 = rDCM->redA2_lsb |
                    ((unsigned short)rDCM->redA2_msb << 8);
                DCM->greenA3 = rDCM->greenA3_lsb |
                    ((unsigned short)rDCM->greenA3_msb << 8);
                DCM->greenA2 = rDCM->greenA2_lsb |
                    ((unsigned short)rDCM->greenA2_msb << 8);
                DCM->blueA3 = rDCM->blueA3_lsb |
                    ((unsigned short)rDCM->blueA3_msb << 8);
                DCM->blueA2 = rDCM->blueA2_lsb |
                    ((unsigned short)rDCM->blueA2_msb << 8);
            } else if(d->type == DESCRIPTOR_TIMINGS) {
                EDID_RawTimings *rTimings = (EDID_RawTimings *)raw->descs[i];
                EDID_Timings *timings = &(edid->descriptors[i].timings);
                timings->type = DESCRIPTOR_TIMINGS;

                for(j = 0; j < 6; j++) {
                    UNPACK_2BIT_TIMING(timings->resolutionsX[j],
                                       timings->aspects[j],
                                       timings->vRefreshes[j],
                                       rTimings->timings[j])
                }
            } else if(d->type == DESCRIPTOR_WHITEP) {
                EDID_RawWhitePoints *rWhitePoints = (EDID_RawWhitePoints *)raw->descs[i];
                EDID_WhitePoints *whitePoints = &(edid->descriptors[i].whitePoints);
                whitePoints->type = DESCRIPTOR_WHITEP;

                for(j = 0; j < 2; j++) {
                    whitePoints->indices[j] = rWhitePoints->whitePoints[j].index;
                    whitePoints->xes[j] = 
                        ((rWhitePoints->whitePoints[j].lsb & 0x0C) >> 2) |
                        ((unsigned short)rWhitePoints->whitePoints[j].x_msb << 2);
                    whitePoints->ys[j] = 
                        (rWhitePoints->whitePoints[j].lsb & 0x03) |
                        ((unsigned short)rWhitePoints->whitePoints[j].y_msb << 2);
                    whitePoints->gammas[j] = rWhitePoints->whitePoints[j].gamma;
                }
                memcpy(whitePoints->unused,
                       rWhitePoints->unused,
                       sizeof(whitePoints->unused));
            } else if(d->type == DESCRIPTOR_NAME) {
                EDID_Text *text = &(edid->descriptors[i].text);
                text->type = DESCRIPTOR_NAME;

                memcpy(text->text, d->text, sizeof(text->text));
            } else if(d->type == DESCRIPTOR_LIMITS) {
                EDID_RawRangeLimits *rRangeLimits = (EDID_RawRangeLimits *)raw->descs[i];
                EDID_RangeLimits *rangeLimits = &(edid->descriptors[i].rangeLimits);
                rangeLimits->type = DESCRIPTOR_LIMITS;

                rangeLimits->vMinRate = rRangeLimits->minvfr_lsb |
                    ((unsigned short)(rRangeLimits->msb & 0x1) << 8);
                rangeLimits->vMaxRate = rRangeLimits->maxvfr_lsb |
                    ((unsigned short)(rRangeLimits->msb & 0x2) << 7);
                rangeLimits->hMinRate = rRangeLimits->minhlr_lsb |
                    ((unsigned short)(rRangeLimits->msb & 0x4) << 6);
                rangeLimits->hMaxRate = rRangeLimits->maxhlr_lsb |
                    ((unsigned short)(rRangeLimits->msb & 0x8) << 5);
                rangeLimits->pixelClock = rRangeLimits->clock * 10;

                rangeLimits->extLimitsType = rRangeLimits->infoType;
                if(rangeLimits->extLimitsType == EXT_LIMITS_GTF) {
                    EDID_RawGTFLimits *rGTFL = (EDID_RawGTFLimits *)rRangeLimits->info;

                    rangeLimits->GTFStart = rGTFL->startFreq;
                    rangeLimits->GTFC = rGTFL->c;
                    rangeLimits->GTFM = rGTFL->m;
                    rangeLimits->GTFK = rGTFL->k;
                    rangeLimits->GTFJ = rGTFL->j;
                } else if(rangeLimits->extLimitsType == EXT_LIMITS_CVT) {
                    EDID_RawCVTLimits *rCVTL = (EDID_RawCVTLimits *)rRangeLimits->info;
    /* CVT */
                    rangeLimits->CVTVersion = rCVTL->version;
                    rangeLimits->CVTExtraPrecision = (rCVTL->clock_active_msb & 0xFC) >> 2;
                    rangeLimits->CVTMaxWidth = rCVTL->active_lsb | 
                        ((unsigned short)(rCVTL->clock_active_msb & 0x03) << 8);
                    rangeLimits->CVTAspects = rCVTL->ar_bitmap;
                    rangeLimits->CVTPrefs = rCVTL->ar_rb_prefs;
                    rangeLimits->CVTScaling = rCVTL->scaling;
                    rangeLimits->CVTPrefRefresh = rCVTL->prefVFR;
                }
            } else if(d->type == DESCRIPTOR_TEXT) {
                EDID_Text *text = &(edid->descriptors[i].text);
                text->type = DESCRIPTOR_TEXT;

                memcpy(text->text, d->text, sizeof(text->text));
            } else if(d->type == DESCRIPTOR_SERIAL) {
                EDID_Text *text = &(edid->descriptors[i].text);
                text->type = DESCRIPTOR_SERIAL;

                memcpy(text->text, d->text, sizeof(text->text));
            } else {
                EDID_Unknown *unknown = &(edid->descriptors[i].unknown);
                unknown->type = DESCRIPTOR_UNKNOWN;

                memcpy(unknown->data, d, sizeof(unknown->data));
            }
        } else { /* DTD */
            EDID_RawDTD *rDTD = (EDID_RawDTD *)raw->descs[i];
            EDID_DTD *DTD = &(edid->descriptors[i].DTD);
            DTD->type = DESCRIPTOR_DTD;

            DTD->clock = rDTD->clock;
            DTD->hActive = ((unsigned short)(rDTD->hActiveBlank_msb & 0xF0) << 4) |
                           (unsigned short)(rDTD->hActive_lsb);
            DTD->hBlanking = ((unsigned short)(rDTD->hActiveBlank_msb & 0x0F) << 8) |
                             (unsigned short)(rDTD->hBlank_lsb);
            DTD->vActive = ((unsigned short)(rDTD->vActiveBlank_msb & 0xF0) << 4) |
                           (unsigned short)(rDTD->vActive_lsb);
            DTD->vBlanking = ((unsigned short)(rDTD->vActiveBlank_msb & 0x0F) << 8) |
                             (unsigned short)(rDTD->vBlank_lsb);
            DTD->hFrontPorch = ((unsigned short)(rDTD->frontSync_msb & 0xC0) << 2) |
                               (unsigned short)(rDTD->hFrontPorch_lsb);
            DTD->hSyncPulse = ((unsigned short)(rDTD->frontSync_msb & 0x30) << 4) |
                              (unsigned short)(rDTD->hSyncPulse_lsb);
            DTD->vFrontPorch = (rDTD->frontSync_msb & 0x0C << 2) |
                               (rDTD->vFrontSync_lsb & 0xF0 >> 4);
            DTD->vSyncPulse = (rDTD->frontSync_msb & 0x03 << 4) |
                              (rDTD->vFrontSync_lsb & 0x0F);
            DTD->width = ((unsigned short)(rDTD->size_msb & 0xF0) << 4) |
                         (unsigned short)(rDTD->width_lsb);
            DTD->height = ((unsigned short)(rDTD->size_msb & 0x0F) << 8) |
                          (unsigned short)(rDTD->height_lsb);
            DTD->hBorder = rDTD->hBorder;
            DTD->vBorder = rDTD->vBorder;
            DTD->features = rDTD->features;
        }
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
                APPEND_STRF("Maximum Active Pixels Per Line: %hu\n", rangeLimits->CVTMaxWidth)
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
