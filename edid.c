#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "edid-structure.h"
#include "growablechar.h"

EDID *unpackEDID(unsigned char *data) {
    EDID_Raw *raw = (EDID_Raw *)data;
    EDID *edid;
    EDID_RawDescriptor *d;
    int i;

    EDID_RawDTD *rDTD;
    EDID_RawNewerModes *rNewerModes;
    EDID_RawCVT *rCVT;
    EDID_RawDCM *rDCM;
    EDID_RawTimings *rTimings;
    EDID_RawWhitePoints *rWhitePoints;
    EDID_RawGTFLimits *rGTFLimits; /* these two go with Range Limits */
    EDID_RawCVTLimits *rCVTLimits;
    EDID_RawRangeLimits *rRangeLimits;

    EDID_DTD *DTD;
    EDID_NewerModes *newerModes;
    EDID_CVT *CVT;
    EDID_DCM *DCM;
    EDID_Timings *timings;
    EDID_WhitePoint *whitePoint;
    EDID_RangeLimits *rangeLimits;
    EDID_Text *text;
    EDID_Unknown *unknown;

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
        edid->resolutionsX[i] = (raw->timings[i].xres + 31) * 8;
        edid->aspects[i] = raw->timings[i].ar_vf & ASPECT_RATIO_BITS;
        edid->vRefreshes[i] = (raw->timings[i].ar_vf & ~ASPECT_RATIO_BITS) + 60;
    }

    for(i = 0; i < 4; i++) {
        d = (EDID_RawDescriptor)raw->descs[i];
        if(d->DTDOrOther == 0) { /* other */
            if(d->type == DESCRIPTOR_NEWER_MODES) {
                rNewerModes = (EDID_RawNewerModes)raw->descs[i];
                newerModes = (EDID_NewerModes)edid->descriptors[i];
                newerModes->type = DESCRIPTOR_NEWER_MODES;
            } else if(d->type == DESCRIPTOR_CVT) {
                rCVT = (EDID_RawCVT)raw->descs[i];
                CVT = (EDID_CVT)edid->descriptors[i];
                CVT->type = DESCRIPTOR_CVT;
            } else if(d->type == DESCRIPTOR_DCM) {
                rDCM = (EDID_RawDCM)raw->descs[i];
                DCM = (EDID_DCM)edid->descriptors[i];
                DCM->type = DESCRIPTOR_DCM;
            } else if(d->type == DESCRIPTOR_TIMINGS) {
                rTimings = (EDID_RawTimings)raw->descs[i];
                timings = (EDID_Timings)edid->descriptors[i];
                timings->type = DESCRIPTOR_TIMINGS;
            } else if(d->type == DESCRIPTOR_WHITEP) {
                rWhitePoints = (EDID_RawWhitePoints)raw->descs[i];
                whitePoints = (EDID_WhitePoints)edid->descriptors[i];
                whitePoints->type = DESCRIPTOR_WHITEP;
            } else if(d->type == DESCRIPTOR_NAME) {
                rText = (EDID_RawDescriptor)raw->descs[i];
                text = (EDID_Text)edid->descriptors[i];
                text->type = DESCRIPTOR_NAME;
            } else if(d->type == DESCRIPTOR_LIMITS) {
                rLimits = (EDID_RawLimits)raw->descs[i];
                limits = (EDID_Limits)edid->descriptors[i];
                limits->type = DESCRIPTOR_LIMITS;
            } else if(d->type == DESCRIPTOR_TEXT) {
                rText = (EDID_RawDescriptor)raw->descs[i];
                text = (EDID_Text)edid->descriptors[i];
                text->type = DESCRIPTOR_TEXT;
            } else if(d->type == DESCRIPTOR_SERIAL) {
                rText = (EDID_RawDscriptor)raw->descs[i];
                text = (EDID_Text)edid->descriptors[i];
                text->type = DESCRIPTOR_SERIAL;
            } else {
                unknown = (EDID_Unknown)edid->descriptors[i];
                unknown->type = DESCRIPTOR_UNKNOWN;

                memcpy(unknown->data, raw, sizeof(EDID_Raw));
            }
        } else { /* DTD */
            rDTD = (EDID_RawDTD)raw->descs[i];
            DTD = (EDID_DTD)edid->descriptors[i];
            DTD->type = DESCRIPTOR_DTD;

            DTD->clock = rDTD->clock;
            DTD->hActive = ((unsigned short)(rDTD->hActiveBlank_msb) & 0xF0 << 4) |
                           (unsigned short)(rDTD->hActive_lsb);
            DTD->hBlanking = ((unsigned short)(rDTD->hActiveBlank_msb) & 0x0F << 8) |
                             (unsigned short)(rDTD->hBlank_lsb);
            DTD->vActive = ((unsigned short)(rDTD->vActiveBlank_msb) & 0xF0 << 4) |
                           (unsigned short)(rDTD->vActive_lsb);
            DTD->vBlanking = ((unsigned short)(rDTD->vActiveBlank_msb) & 0x0F << 8) |
                             (unsigned short)(rDTD->vBlank_lsb);
            DTD->hFrontPorch = ((unsigned short)(rDTD->frontSync_msb) & 0xC0 << 2) |
                               (unsigned short)(rDTD->hFrontPorch_lsb);
            DTD->hSyncPulse = ((unsigned short)(rDTD->frontSync_msb) & 0x30 << 4) |
                              (unsigned short)(rDTD->hSyncPulse_lsb);
            DTD->vFrontPorch = ((unsigned short)(rDTD->frontSync_msb) & 0x0C << 2) |
                               ((unsigned short)(rDTD->vFrontSync_lsb) & 0xF0 >> 4);
            DTD->vSyncPulse = ((unsigned short)(rDTD->frontSync_msb) & 0x03 << 4) |
                              ((unsigned short)(rDTD->vFrontSync_lsb) & 0x0F);
            DTD->width = ((unsigned short)(rDTD->size_msb) & 0xF0 << 4) |
                         (unsigned short)(rDTD->width_lsb);
            DTD->height = ((unsigned short)(rDTD->size_msb) & 0x0F << 8) |
                          (unsigned short)(rDTD->height_lsb);
            DTD->hBorder = rDTD->hBorder;
            DTD->vBorder = rDTD->vborder;
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
char *parseEDID(EDID *edid) {
    GrowableChar *gc;
    int i;

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
        APPEND_STR("Black-to-Black Setup Expected: ")
        if(edid->attribs & BLACK_TO_BLACK_BIT) {
            APPEND_STR("Yes\n")
        } else {
            APPEND_STR("No\n")
        }
        APPEND_STR("Separate Sync Supported: ")
        if(edid->attribs & SEPARATE_SYNC_BIT) {
            APPEND_STR("Yes\n")
        } else {
            APPEND_STR("No\n")
        }
        APPEND_STR("Composite HSync Supported: ")
        if(edid->attribs & COMPOSITE_SYNC_BIT) {
            APPEND_STR("Yes\n")
        } else {
            APPEND_STR("No\n")
        }
        APPEND_STR("Sync On Green Supported: ")
        if(edid->attribs & SYNC_ON_GREEN_BIT) {
            APPEND_STR("Yes\n")
        } else {
            APPEND_STR("No\n")
        }
        APPEND_STR("VSync pulse must be serrated when composite or sync-on-green is used: ")
        if(edid->attribs & SERRATED_VSYNC_BIT) {
            APPEND_STR("Yes\n")
        } else {
            APPEND_STR("No\n")
        }
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

    APPEND_STR("DPMS Standby Supported: ")
    if(edid->features & STANDBY_BIT) {
        APPEND_STR("Yes\n")
    } else {
        APPEND_STR("No\n")
    }
    APPEND_STR("DPMS Suspend Supported: ")
    if(edid->features & SUSPEND_BIT) {
        APPEND_STR("Yes\n")
    } else {
        APPEND_STR("No\n")
    }
    APPEND_STR("DPMS Active-off Supported: ")
    if(edid->features & ACTIVEOFF_BIT) {
        APPEND_STR("Yes\n")
    } else {
        APPEND_STR("No\n")
    }
    if(edid->inputType == INPUT_DIGITAL) {
        APPEND_STRF("Digital Display Type: %s\n", dTypeToStr(edid->dType))
    } else if(edid->inputType == INPUT_ANALOG) {
        APPEND_STRF("Analog Display Type: %s\n", aTypeToStr(edid->aType))
    }        
    APPEND_STR("sRGB Color Space: ")
    if(edid->features & SRGB_BIT) {
        APPEND_STR("Yes\n")
    } else {
        APPEND_STR("No\n")
    }
    if(versionCompare(edid->verMajor, edid->verMinor, 1, 3) < 0) {
        APPEND_STR("Preferred Timing Mode is in First Destailed Timing Descriptor: ")
    } else {
        APPEND_STR("Preferred Timing Mode is Native: ")
    }
    if(edid->features & PREFERRED_BIT) {
        APPEND_STR("Yes\n")
    } else {
        APPEND_STR("No\n")
    }
    APPEND_STR("Default GTF Supported: ")
    if(edid->features & GTF_BIT) {
        APPEND_STR("Yes\n")
    } else {
        APPEND_STR("No\n")
    }

    APPEND_STR("\n*** Chromaticity Coordinates ***\n")
    APPEND_STRF("Red Point (x, y): (%hu, %hu)\n", edid->redPointX, edid->redPointY)
    APPEND_STRF("Green Point (x, y): (%hu, %hu)\n", edid->greenPointX, edid->greenPointY)
    APPEND_STRF("Blue Point (x, y): (%hu, %hu)\n", edid->bluePointX, edid->bluePointY)
    APPEND_STRF("White Point (x, y): (%hu, %hu)\n", edid->whitePointX, edid->whitePointY)

    APPEND_STR("\n*** Established Timings ***\n")
    APPEND_STR("720x400@70: ")
    if(edid->oldModes & MODE_720_400_70) {
        APPEND_STR("Yes\n")
    } else {
        APPEND_STR("No\n")
    }
    APPEND_STR("720x400@88: ")
    if(edid->oldModes & MODE_720_400_88) {
        APPEND_STR("Yes\n")
    } else {
        APPEND_STR("No\n")
    }
    APPEND_STR("640x480@60: ")
    if(edid->oldModes & MODE_640_480_60) {
        APPEND_STR("Yes\n")
    } else {
        APPEND_STR("No\n")
    }
    APPEND_STR("640x480@67: ")
    if(edid->oldModes & MODE_640_480_67) {
        APPEND_STR("Yes\n")
    } else {
        APPEND_STR("No\n")
    }
    APPEND_STR("640x480@72: ")
    if(edid->oldModes & MODE_640_480_72) {
        APPEND_STR("Yes\n")
    } else {
        APPEND_STR("No\n")
    }
    APPEND_STR("640x480@75: ")
    if(edid->oldModes & MODE_640_480_75) {
        APPEND_STR("Yes\n")
    } else {
        APPEND_STR("No\n")
    }
    APPEND_STR("800x600@56: ")
    if(edid->oldModes & MODE_800_600_56) {
        APPEND_STR("Yes\n")
    } else {
        APPEND_STR("No\n")
    }
    APPEND_STR("800x600@60: ")
    if(edid->oldModes & MODE_800_600_60) {
        APPEND_STR("Yes\n")
    } else {
        APPEND_STR("No\n")
    }
    APPEND_STR("800x600@72: ")
    if(edid->oldModes & MODE_800_600_72) {
        APPEND_STR("Yes\n")
    } else {
        APPEND_STR("No\n")
    }
    APPEND_STR("800x600@75: ")
    if(edid->oldModes & MODE_800_600_75) {
        APPEND_STR("Yes\n")
    } else {
        APPEND_STR("No\n")
    }
    APPEND_STR("832x624@75: ")
    if(edid->oldModes & MODE_832_624_75) {
        APPEND_STR("Yes\n")
    } else {
        APPEND_STR("No\n")
    }
    APPEND_STR("1024x768@87 Interlaced: ")
    if(edid->oldModes & MODE_1024_768_87I) {
        APPEND_STR("Yes\n")
    } else {
        APPEND_STR("No\n")
    }
    APPEND_STR("1024x768@60: ")
    if(edid->oldModes & MODE_1024_768_60) {
        APPEND_STR("Yes\n")
    } else {
        APPEND_STR("No\n")
    }
    APPEND_STR("1024x768@72: ")
    if(edid->oldModes & MODE_1024_768_72) {
        APPEND_STR("Yes\n")
    } else {
        APPEND_STR("No\n")
    }
    APPEND_STR("1024x768@75: ")
    if(edid->oldModes & MODE_1024_768_75) {
        APPEND_STR("Yes\n")
    } else {
        APPEND_STR("No\n")
    }
    APPEND_STR("1280x1024@75: ")
    if(edid->oldModes & MODE_1280_1024_75) {
        APPEND_STR("Yes\n")
    } else {
        APPEND_STR("No\n")
    }
    APPEND_STR("1152x870@75: ")
    if(edid->oldModes & MODE_1152_870_75) {
        APPEND_STR("Yes\n")
    } else {
        APPEND_STR("No\n")
    }
    APPEND_STR("\n*** Standard Timing Information ***\n")
    for(i = 0; i < 8; i++) {
        if(isDefined2BitMode(edid->resolutionsX[i], edid->aspects[i], edid->vRefreshes[i])) {
            APPEND_STRF("%hux", edid->resolutionsX[i])
            if(edid->aspects[i] == ASPECT_16_10_1_1) {
                if(versionCompare(edid->verMajor, edid->verMinor, 1, 3) < 0) { /* 1:1 */
                    APPEND_STRF("%hu", edid->resolutionsX[i])
                } else {
                    APPEND_STRF("%hu", edid->resolutionsX[i] / 16 * 10)
                }
            } else if(edid->aspects[i] == ASPECT_4_3) {
                APPEND_STRF("%hu", edid->resolutionsX[i] / 4 * 3)
            } else if(edid->aspects[i] == ASPECT_5_4) {
                APPEND_STRF("%hu", edid->resolutionsX[i] / 5 * 4)
            } else if(edid->aspects[i] == ASPECT_16_9) {
                APPEND_STRF("%hu", edid->resolutionsX[i] / 16 * 9)
            }
            APPEND_STRF("@%hhu\n", edid->vRefreshes[i]);
        } else {
            APPEND_STR("Unused Mode Field\n")
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

    for(i = 0; i < 4; i++) {
        
}
#undef APPEND_STR
