#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <linux/i2c-dev.h>
#include <i2c/smbus.h>

#include "eedid.h"

int open_i2c_dev(const char *devpath) {
    int i2cdev;
    unsigned long funcs;

    i2cdev = open(devpath, O_RDWR);
    if (i2cdev < 0) {
        fprintf(stderr, "Couldn't open device %s: %s\n", devpath, strerror(errno));
        return(-1);
    }

    if(ioctl(i2cdev, I2C_FUNCS, &funcs) < 0) {
        fprintf(stderr, "Couldn't get i2c functions: %s\n", strerror(errno));
        close(i2cdev);
        return(-1);
    }

    if(!(funcs & I2C_FUNC_SMBUS_READ_BYTE_DATA) ||
       !(funcs & I2C_FUNC_SMBUS_WRITE_BYTE) ||
       !(funcs & I2C_FUNC_I2C)/* ||
       !(funcs & I2C_FUNC_PROTOCOL_MANGLING)*/) {
        fprintf(stderr, "Missing functions for this i2c device.\n");
        close(i2cdev);
        return(-1);
    }

    return(i2cdev);
}

int set_i2c_slave(int i2cdev, int addr) {
    if(ioctl(i2cdev, I2C_SLAVE, addr) < 0) {
        fprintf(stderr, "Couldn't set i2c slave addr %i: %s\n", addr, strerror(errno));
        return(-1);
    }

    return(0);
}

int read_edid(int i2cdev, unsigned char *data) {
    int i;
    int ret;

    if(set_i2c_slave(i2cdev, EDID_DATA_ADDR) < 0) {
        return(-1);
    }

    for(i = 0; i < 256; i++) {
        ret = i2c_smbus_read_byte_data(i2cdev, i);
        if(ret < 0) {
            fprintf(stderr, "Failed to read byte %i.\n", i);
            return(-1);
        }
        data[i] = (unsigned char)ret;
    }

    return(0);
}

int read_eedid_block(int i2cdev, unsigned char block, unsigned char *data) {
    struct i2c_rdwr_ioctl_data msgset;
    struct i2c_msg msgs[EDID_PAGE_I2C_MSGS + 1]; /* 1 more to set the page */
    int i;

    unsigned char stuff[EDID_PAGE_SIZE + 1]; /* needs 1 extra byte for length */

/*    memset(stuff, 127, EDID_PAGE_SIZE + 1);
*/
    msgset.msgs = msgs;
    msgset.nmsgs = EDID_PAGE_I2C_MSGS + 1;

    msgs[0].addr = EEDID_INDEX_ADDR;
    msgs[0].flags = 0; /* write */
    msgs[0].len = 1;
    msgs[0].buf = &block;

    /* need to read 256 bytes one byte at a time... */
/*    for(i = 1; i < 257; i++) {
        msgs[i].addr = EDID_DATA_ADDR;
        msgs[i].flags = I2C_M_RD;
        msgs[i].len = 1;
        msgs[i].buf = &(data[i - 1]);
    }
*/

    for(i = 0; i < EDID_PAGE_I2C_MSGS; i++) {
        msgs[i+1].addr = EDID_DATA_ADDR;
        msgs[i+1].flags = I2C_M_RD;
        msgs[i+1].len = I2C_SMBUS_BLOCK_MAX;
        msgs[i+1].buf = &(stuff[i * I2C_SMBUS_BLOCK_MAX]);
    }

    if(ioctl(i2cdev, I2C_RDWR, &msgset) < 0) {
        fprintf(stderr, "i2c combined transaction failed: %s\n", strerror(errno));
        return(-1);
    }

    memcpy(data, stuff, EDID_PAGE_SIZE);

    return(0);
}
