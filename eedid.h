#include <linux/i2c.h>

#define EEDID_INDEX_ADDR (0x30)
#define EDID_DATA_ADDR (0x50)
#define EDID_PAGE_SIZE (256)
#define EDID_PAGE_I2C_MSGS (EDID_PAGE_SIZE / I2C_SMBUS_BLOCK_MAX)

int open_i2c_dev(const char *devpath);
int set_i2c_slave(int i2cdev, int addr);
int read_edid(int i2cdev, unsigned char *data);
int read_eedid_block(int i2cdev, unsigned char block, unsigned char *data);
