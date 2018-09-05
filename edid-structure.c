#include "edid-structure.h"

const char EDID_MAGIC[8] = {
    0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00
};

const char EDID_CONVTABLE[32] = {
    ' ', 'A', 'B', 'C', 'D', 'E', 'F', 'G',
    'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O',
    'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W',
    'X', 'Y', 'Z', '1', '2', '3', '4', '5'
}; /* numbers are probably incorrect but they're simple and unique */

const char *EDID_INVALID = "Invalid";

const char *EDID_INPUT_TYPES[2] = {
    "Analog",
    "Digital"
};

const char *EDID_BIT_DEPTHS[8] = {
    "Undefined",
    "6 bit",
    "8 bit",
    "10 bit",
    "12 bit",
    "14 bit",
    "16 bit",
    "Reserved"
};

const char *EDID_INTERFACES[5] = {
    "Undefined",
    "HDMIa",
    "HDMIb",
    "MDDI",
    "DisplayPort"
};

const char *EDID_LEVELS[4] = {
    "+0.7V, -0.3V",
    "+0.714V, -0.286V",
    "+1.0V, -0.4V",
    "+0.6V, 0.0V"
};

const char *EDID_DIGITAL_TYPES[4] = {
    "RGB 4:4:4",
    "RGB 4:4:4, YCrCb 4:4:4",
    "RGB 4:4:4, YCrCb 4:2:2",
    "RGB 4:4:4, YCrCb 4:4:4, YCrCb 4:2:2"
};

const char *EDID_ANALOG_TYPES[4] = {
    "Monochrome or Grayscale",
    "RGB Color",
    "Non-RGB Color",
    "Undefined"
};

const char *EDID_STEREO_MODES[7] = {
    "None",
    "Field Sequential",
    "Right Image On Even Lines",
    "Opposite Field Sequential",
    "Left Image On Even Lines",
    "4-Way Interleaved",
    "Side By Side"
};

const char *EDID_PREFERRED_RATES[4] = {
    "50 Hz",
    "60 Hz",
    "75 Hz",
    "85 Hz"
};

const char *EDID_CVT_ASPECTS[4] = {
    "16:10",
    "4:3",
    "5:4",
    "16:9"
};

const char *EDID_EXT_LIMIT_TYPES[4] = {
    "Default GTF",
    "No Timing Info",
    "Secondary GTF",
    "CVT"
};

const char *EDID_CVT_LIMIT_PREF_ASPECT[5] = {
    "4:3",
    "16:9",
    "16:10",
    "5:4",
    "15:9"
};

void unpackManuID(unsigned char *manuID, const unsigned char *data) {
    manuID[0] = EDID_CONVTABLE[(data[0] & 0x7C) >> 2];
    manuID[1] = EDID_CONVTABLE[((data[0] & 0x03) << 3) | ((data[1] & 0xE0) >> 5)];
    manuID[2] = EDID_CONVTABLE[data[1] & 0x1F];
    manuID[3] = '\0';
}

const char *inputTypeToStr(EDID_InputType i) {
    if(i == INPUT_ANALOG) {
        return(EDID_INPUT_TYPES[0]);
    } else if(i == INPUT_DIGITAL) {
        return(EDID_INPUT_TYPES[1]);
    }
    
    return(EDID_INVALID);
}

const char *bitDepthToStr(EDID_BitDepth b) {
    if(b == BITS_UNDEF) {
        return(EDID_BIT_DEPTHS[0]);
    } else if(b == BITS_6) {
        return(EDID_BIT_DEPTHS[1]);
    } else if(b == BITS_8) {
        return(EDID_BIT_DEPTHS[2]);
    } else if(b == BITS_10) {
        return(EDID_BIT_DEPTHS[3]);
    } else if(b == BITS_12) {
        return(EDID_BIT_DEPTHS[4]);
    } else if(b == BITS_14) {
        return(EDID_BIT_DEPTHS[5]);
    } else if(b == BITS_16) {
        return(EDID_BIT_DEPTHS[6]);
    } else if(b == BITS_RESERVED) {
        return(EDID_BIT_DEPTHS[7]);
    }

    return(EDID_INVALID);
}

const char *interfaceToStr(EDID_Interface i) {
    if(i == INTERF_UNDEFINED) {
        return(EDID_INTERFACES[0]);
    } else if(i == INTERF_HDMIA) {
        return(EDID_INTERFACES[1]);
    } else if(i == INTERF_HDMIB) {
        return(EDID_INTERFACES[2]);
    } else if(i == INTERF_MDDI) {
        return(EDID_INTERFACES[3]);
    } else if(i == INTERF_DP) {
        return(EDID_INTERFACES[4]);
    }

    return(EDID_INVALID);
}

const char *voltsToStr(EDID_Levels l) {
    if(l == LEVELS_07_03) {
        return(EDID_LEVELS[0]);
    } else if(l == LEVELS_0714_0286) {
        return(EDID_LEVELS[1]);
    } else if(l == LEVELS_10_04) {
        return(EDID_LEVELS[2]);
    } else if(l == LEVELS_07_0) {
        return(EDID_LEVELS[3]);
    }

    return(EDID_INVALID);
}

const char *dTypeToStr(EDID_DigitalType t) {
    if(t == TYPE_RGB) {
        return(EDID_DIGITAL_TYPES[0]);
    } else if(t == TYPE_RGB_YCRCB_444) {
        return(EDID_DIGITAL_TYPES[1]);
    } else if(t == TYPE_RGB_YCRCB_422) {
        return(EDID_DIGITAL_TYPES[2]);
    } else if(t == TYPE_RGB_YCRCB_BOTH) {
        return(EDID_DIGITAL_TYPES[3]);
    }

    return(EDID_INVALID);
}

const char *aTypeToStr(EDID_AnalogType t) {
    if(t == TYPE_MONOCHROME) {
        return(EDID_ANALOG_TYPES[0]);
    } else if(t == TYPE_COLOR) {
        return(EDID_ANALOG_TYPES[1]);
    } else if (t == TYPE_NONRGB) {
        return(EDID_ANALOG_TYPES[2]);
    } else if (t == TYPE_UNDEF) {
        return(EDID_ANALOG_TYPES[3]);
    }

    return(EDID_INVALID);
}

int versionCompare(unsigned char major1, unsigned char minor1,
                    unsigned char major2, unsigned char minor2) {
    short ver1 = (short)major1 << 8 | minor1;
    short ver2 = (short)major2 << 8 | minor2;

    return(ver1 - ver2);
}

int isDefined2ByteMode(unsigned short rx, EDID_Aspect a, unsigned char vr) {
    /* 01 01 */
    if(rx == 256 && a == 0 && vr == 61) {
        return(0);
    }

    return(1);
}

const char *stereoModeToStr(EDID_StereoMode s) {
    if(s == STEREO_NONE) {
        return(EDID_STEREO_MODES[0]);
    } else if( s == STEREO_FIELD) {
        return(EDID_STEREO_MODES[1]);
    } else if( s == STEREO_REVEN) {
        return(EDID_STEREO_MODES[2]);
    } else if( s == STEREO_FIELD_OPPOSITE) {
        return(EDID_STEREO_MODES[3]);
    } else if( s == STEREO_LEVEN) {
        return(EDID_STEREO_MODES[4]);
    } else if( s == STEREO_4WAY) {
        return(EDID_STEREO_MODES[5]);
    } else if( s == STEREO_SBS) {
        return(EDID_STEREO_MODES[6]);
    }

    return(EDID_INVALID);
}

const char *preferredRateToStr(EDID_PreferredRate r) {
    if(r == PRATE_50) {
        return(EDID_PREFERRED_RATES[0]);
    } else if(r == PRATE_60) {
        return(EDID_PREFERRED_RATES[1]);
    } else if(r == PRATE_75) {
        return(EDID_PREFERRED_RATES[2]);
    } else if(r == PRATE_85) {
        return(EDID_PREFERRED_RATES[3]);
    }

    return(EDID_INVALID);
}

const char *CVTAspectToStr(EDID_CVTAspect a) {
    if(a == CVT_ASPECT_16_10) {
        return(EDID_CVT_ASPECTS[0]);
    } else if(a == CVT_ASPECT_4_3) {
        return(EDID_CVT_ASPECTS[1]);
    } else if(a == CVT_ASPECT_5_4) {
        return(EDID_CVT_ASPECTS[2]);
    } else if(a == CVT_ASPECT_16_9) {
        return(EDID_CVT_ASPECTS[3]);
    }

    return(EDID_INVALID);
}

const char *extLimitsTypeToStr(EDID_ExtLimitsType t) {
    if (t == EXT_LIMITS_DEFAULT) {
        return(EDID_EXT_LIMIT_TYPES[0]);
    } else if(t == EXT_LIMITS_NONE) {
        return(EDID_EXT_LIMIT_TYPES[1]);
    } else if(t == EXT_LIMITS_GTF) {
        return(EDID_EXT_LIMIT_TYPES[2]);
    } else if(t == EXT_LIMITS_CVT) {
        return(EDID_EXT_LIMIT_TYPES[3]);
    }

    return(EDID_INVALID);
}

const char *CVTLimitPrefAspectToStr(EDID_CVTLimitPrefAspect p) {
    if(p == CVT_LIMIT_ASPECT_4_3) {
        return(EDID_CVT_LIMIT_PREF_ASPECT[0]);
    } else if(p == CVT_LIMIT_ASPECT_16_9) {
        return(EDID_CVT_LIMIT_PREF_ASPECT[1]);
    } else if(p == CVT_LIMIT_ASPECT_16_10) {
        return(EDID_CVT_LIMIT_PREF_ASPECT[2]);
    } else if(p == CVT_LIMIT_ASPECT_5_4) {
        return(EDID_CVT_LIMIT_PREF_ASPECT[3]);
    } else if(p == CVT_LIMIT_ASPECT_15_9) {
        return(EDID_CVT_LIMIT_PREF_ASPECT[4]);
    }

    return(EDID_INVALID);
}
