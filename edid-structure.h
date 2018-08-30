extern const char convtable[32];

#define INPUTTYPE_BITS (7)

typedef enum {
    INPUT_ANALOG = 0x80,
    INPUT_DIGITAL = 0x00
} EDID_InputType;

#define BITDEPTH_BITS (0x70)
#define INTERFACE_BITS (0x0F)

typedef enum {
    BITS_UNDEF = 0x00,
    BITS_6 = 0x10,
    BITS_8 = 0x20,
    BITS_10 = 0x30,
    BITS_12 = 0x40,
    BITS_14 = 0x50,
    BITS_16 = 0x60,
    BITS_RESERVED = 0x70
} EDID_BitDepth;

typedef enum {
    INTERF_UNDEFINED = 0x00,
    INTERF_HDMIA = 0x01,
    INTERF_HDMIB = 0x02,
    INTERF_MDDI = 0x03,
    INTERF_DP = 0x04,
    INTERF_RESERVED = 0x05
} EDID_Interface;

#define LEVELS_BITS (0x60)

typedef enum {
    LEVELS_07_03 = 0x00,
    LEVELS_0714_0286 = 0x20,
    LEVELS_10_04 = 0x40,
    LEVELS_07_0 = 0x60,
} EDID_Levels;

/* analog signal attributes */
#define BLACK_TO_BLACK_BIT (0x10)
#define SEPARATE_SYNC_BIT  (0x08)
#define COMPOSITE_SYNC_BIT (0x04)
#define SYNC_ON_GREEN_BIT  (0x02)
#define SERRATED_VSYNC_BIT (0x01)

/* features */
#define TYPE_BITS (0x18)

typedef enum {
    TYPE_RGB = 0x00,
    TYPE_RGB_YCRCB_444 = 0x08,
    TYPE_RGB_YCRCB_422 = 0x10,
    TYPE_RGB_YCRCB_BOTH = 0x18
} EDID_DigitalType;

typedef enum {
    TYPE_MONOCHROME = 0x00,
    TYPE_RGB = 0x08,
    TYPE_NONRGB = 0x10,
    TYPE_UNDEF = 0x18
} EDID_AnalogType;

#define STANDBY_BIT    (0x80)
#define SUSPEND_BIT    (0x40)
#define ACTIVEOFF_BIT  (0x20)
#define SRGB_BIT       (0x04)
#define PREFERRED_BIT  (0x02)
#define CONTINUOUS_BIT (0x01)

#define MODE_720_400_70   (0x800000)
#define MODE_720_400_88   (0x400000)
#define MODE_640_480_60   (0x200000)
#define MODE_640_480_67   (0x100000)
#define MODE_640_480_72   (0x080000)
#define MODE_640_480_75   (0x040000)
#define MODE_800_600_56   (0x020000)
#define MODE_800_600_60   (0x010000)
#define MODE_800_600_72   (0x008000)
#define MODE_800_600_75   (0x004000)
#define MODE_832_624_75   (0x002000)
#define MODE_1024_768_87I (0x001000)
#define MODE_1024_768_60  (0x000800)
#define MODE_1024_768_72  (0x000400)
#define MODE_1024_768_75  (0x000200)
#define MODE_1280_1024_75 (0x000100)
#define MODE_1152_870_75  (0x000080)
#define MODE_MANUFACTURER (0x00007F)

#define ASPECT_RATIO_BITS (0xC0)

typedef enum {
    ASPECT_16_10 = 0x00,
    ASPECT_4_3 = 0x40,
    ASPECT_5_4 = 0x80,
    ASPECT_16_9 = 0xC0
} EDID_Aspect;

typedef enum {
    DESCRIPTOR_DTD = 0x00,
    DESCRIPTOR_NEWER_MODES = 0xF7,
    DESCRIPTOR_CVT = 0xF8,
    DESCRIPTOR_DCM = 0xF9,
    DESCRIPTOR_TIMING = 0xFA,
    DESCRIPTOR_WHITEP = 0xFB,
    DESCRIPTOR_NAME = 0xFC,
    DESCRIPTOR_LIMITS = 0xFD,
    DESCRIPTOR_TEXT = 0xFE,
    DESCRIPTOR_SERIAL = 0xFF
} EDID_DescriptorType;

#define INTERLACED_BIT   (0x80)
#define STEREO_BITS      (0x60)
#define STEREO_EXTRA_BIT (0x01)
#define STEREO_BITS_SHIFT (5)

#define DIGITAL_SYNC_BIT (0x10)
#define SYNC_TYPE_BIT    (0x08) /* analog: bipolar composite, digital: composite/separate */
/* analog sync */
#define DTD_SERRATED_VSYNC_BIT (0x04) /* digital separate sync too */
#define SYNC_ALL_LINES_BIT     (0x02)
/* digital composite sync */
#define VSYNC_POLARITY_BIT (0x04)
#define DTD_RESERVED_BIT   (0x02)
/* digital separate sync */
#define HSYNC_POLARITY_BIT (0x02)

typedef enum {
    STEREO_NONE = 0x0, /* 00 0 */
    STEREO_INVALID = 0x1, /* 00 1 */
    STEREO_FIELD = 0x2, /* 01 0 */
    STEREO_REVEN = 0x3 /* 01 1 */
    STEREO_SIMILAR = 0x4 /* 10 0 */
    STEREO_LEVEN = 0x5 /* 10 1 */
    STEREO_4WAY = 0x6 /* 11 0 */
    STEREO_SBS = 0x7 /* 11 1 */
} EDID_StereoMode;

typedef struct {
    EDID_DescriptorType type;
    unsigned short clock;
    unsigned short hActive;
    unsigned short hBlanking;
    unsigned short hFrontPorch;
    unsigned short hSyncPulse;
    unsigned short hSize;
    unsigned short hBorder;
    unsigned short vActive;
    unsigned short vBlanking;
    unsigned short vFrontPorch;
    unsigned short vSyncPulse;
    unsigned short vSize;
    unsigned short vBorder;
    unsigned char features;
} EDID_DTD;

#define NMODE_640_350_85    (0x800000000000)
#define NMODE_640_400_85    (0x400000000000)
#define NMODE_720_400_85    (0x200000000000)
#define NMODE_640_480_85    (0x100000000000)
#define NMODE_640_480_60    (0x080000000000)
#define NMODE_800_600_85    (0x040000000000)
#define NMODE_1024_768_85   (0x020000000000)
#define NMODE_1152_864_85   (0x010000000000)

#define NMODE_1280_768_60R  (0x008000000000)
#define NMODE_1280_768_60   (0x004000000000)
#define NMODE_1280_768_75   (0x002000000000)
#define NMODE_1280_768_85   (0x001000000000)
#define NMODE_1280_960_60   (0x000800000000)
#define NMODE_1280_960_85   (0x000400000000)
#define NMODE_1280_1024_60  (0x000200000000)
#define NMODE_1280_1024_85  (0x000100000000)

#define NMODE_1360_768_60R  (0x000080000000)
#define NMODE_1360_768_60   (0x000040000000) /* maybe WikiP transcription error */
#define NMODE_1440_900_60R  (0x000020000000)
#define NMODE_1440_900_75   (0x000010000000)
#define NMODE_1440_900_85   (0x000008000000)
#define NMODE_1440_1050_60R (0x000004000000)
#define NMODE_1440_1050_60  (0x000002000000)
#define NMODE_1440_1050_75  (0x000001000000)

#define NMODE_1440_1050_85  (0x000000800000)
#define NMODE_1680_1050_60R (0x000000400000)
#define NMODE_1680_1050_60  (0x000000200000)
#define NMODE_1680_1050_75  (0x000000100000)
#define NMODE_1680_1050_85  (0x000000080000)
#define NMODE_1600_1200_60  (0x000000040000)
#define NMODE_1600_1200_65  (0x000000020000)
#define NMODE_1600_1200_70  (0x000000010000)

#define NMODE_1600_1200_75  (0x000000008000)
#define NMODE_1600_1200_85  (0x000000004000)
#define NMODE_1792_1344_60  (0x000000002000)
#define NMODE_1792_1344_75  (0x000000001000)
#define NMODE_1856_1392_60  (0x000000000800)
#define NMODE_1856_1392_75  (0x000000000400)
#define NMODE_1920_1200_60R (0x000000000200)
#define NMODE_1920_1200_60  (0x000000000100)

#define NMODE_1920_1200_75  (0x000000000080)
#define NMODE_1920_1200_85  (0x000000000040) /* another maybe transcription error */
#define NMODE_1920_1440_60  (0x000000000020)
#define NMODE_1920_1440_75  (0x000000000010)
#define NMODE_UNUSED_BITS   (0x00000000000F)

typedef struct {
    EDID_DescriptorType type;
    unsigned char version;
    unsigned long long int modesBitmap;
} EDID_NewerModes;

typedef enum {
    PRATE_50 = 0x00,
    PRATE_60 = 0x04,
    PRATE_75 = 0x08,
    PRATE_85 = 0x0C
} EDID_PreferredRate;

typedef enum {
    CVT_ASPECT_16_10 = 0x00,
    CVT_ASPECT_4_3 = 0x20,
    CVT_ASPECT_5_4 = 0x40,
    CVT_ASPECT_16_9 = 0x60
} EDID_CVTAspect;

#define CVT_RATE_50  (0x10)
#define CVT_RATE_60  (0x08)
#define CVT_RATE_75  (0x04)
#define CVT_RATE_85  (0x02)
#define CVT_RATE_60R (0x01)

typedef struct {
    EDID_DescriptorType type;
    unsigned char version;

    unsigned short addressableLines[4];
    EDID_PreferredRate preferredRate[4];
    EDID_CVTAspect aspects[4]
    unsigned char rates[4];
} EDID_CVT;

typedef struct {
    EDID_DescriptorType type;

    unsigned char version;
    unsigned short redA3;
    unsigned short redA2;
    unsigned short greenA3;
    unsigned short greenA2;
    unsigned short blueA3;
    unsigned short blueA2;
} EDID_DCM;

typedef struct {
    EDID_DescriptorType type;

    int resolutionsX[6];
    EDID_Aspect aspects[6];
    int vRefreshes[6];
} EDID_Timing;

typedef struct {
    EDID_DescriptorType type;
    
    unsigned char indices[2];
    unsigned short xes[2];
    unsigned short ys[2];
    unsigned char gammas[2];
} EDID_WhitePoint;

typedef enum {
    EXT_LIMITS_DEFAULT = 0x00,
    EXT_LIMITS_NONE = 0x01,
    EXT_LIMITS_GTF = 0x02,
    EXT_LIMITS_CVT = 0x04
} EDID_ExtLimitsType;

#define CVT_ASPECT_4_3_BIT   (0x80)
#define CVT_ASPECT_16_9_BIT  (0x40)
#define CVT_ASPECT_16_10_BIT (0x20)
#define CVT_ASPECT_5_4_BIT   (0x10)
#define CVT_ASPECT_15_9_BIT  (0x08)

typedef enum {
    CVT_LIMIT_ASPECT_4_3 = 0x00,
    CVT_LIMIT_ASPECT_16_9 = 0x20,
    CVT_LIMIT_ASPECT_16_10 = 0x40,
    CVT_LIMIT_ASPECT_5_4 = 0x60,
    CVT_LIMIT_ASPECT_15_9 = 0x80
} EDID_CVTLimitPrefAspect;

#define CVT_SCALE_XSHRINK_BIT (0x80)
#define CVT_SCALE_XGROW_BIT   (0x40)
#define CVT_SCALE_YSHRINK_BIT (0x20)
#define CVT_SCALE_YGROW_BIT   (0x10)

typedef struct {
    EDID_DescriptorType type;
    unsigned short vMinRate;
    unsigned short vMaxRate;
    unsigned short hMinRate;
    unsigned short hMaxRate;
    unsigned char pixelClock;
    EDID_ExtLimitsType extLimitsType;

    /* GTF, no idea, info costs money */
    unsigned char GTFStart;
    unsigned char GTFC;
    unsigned char GTFM;
    unsigned char GTFK;
    unsigned char GTFJ;

    /* CVT */
    unsigned char CVTVersion;
    unsigned char CVTExtraPrecision;
    unsigned short CVTMaxWidth;
    unsigned char CVTAspects;
    unsigned char CVTPrefs;
    unsigned char CVTScaling;
    unsigned char CVTPrefRefresh;
} EDID_RangeLimits;

typedef struct {
    EDID_DescriptorType type;

    char text[13];
} EDID_Text;

typedef union
    EDID_DescriptorType type;
    EDID_DTD DTD;
    EDID_NewerModes newerModes;
    EDID_CVT CVT;
    EDID_DCM DCM;
    EDID_Timing timing;
    EDID_WhitePoint whitePoint;
    EDID_RangeLimits rangeLimits;
    EDID_Text text;
} EDID_Descriptor;

typedef struct { /* represents interpreted data, not literal structure */
    /* header */
    char manuID[4];
    unsigned short productID;
    unsigned int serial;
    unsigned char week;
    unsigned char year;
    unsigned char verMajor;
    unsigned char verMinor;

    /* basic display parameters */
    EDID_InputType inputType;
    /* digital */
    EDID_BitDepth bitDepth;
    EDID_Interface interface;
    /* analog */
    EDID_Levels levels;
    unsigned char attribs;
    
    unsigned char hsize;
    unsigned char vsize;
    unsigned char gamma;
    unsigned char features;

    /* chromaticity coordinates */
    unsigned short whitePointX;
    unsigned short whitePointY;
    unsigned short redPointX;
    unsigned short redPointY;
    unsigned short greenPointX;
    unsigned short greenPointY;
    unsigned short bluePointX;
    unsigned short bluePointY;

    /* old timing bitmap */
    unsigned int oldModes;

    /* standard timing information */
    int resolutionsX[8];
    EDID_Aspect aspects[8];
    int vRefreshes[8];

    EDID_Descriptor descriptors[4];

    unsigned char extensions;

    unsigned char checksum;
    int checksumGood;
} EDID;
