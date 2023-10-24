
/*
This code is based on: 
- https://github.com/Embetronicx/Tutorials/tree/master/Linux/Device_Driver/I2C-Linux-Device-Driver/SSD1306_I2C_Driver
- https://embetronicx.com/tutorials/linux/device-drivers/ssd1306-i2c-linux-device-driver-using-raspberry-pi/
*/


#include <linux/module.h>
#include <linux/i2c.h>


#include "ssd1306_util.h"
#include "ssd1306_commands.h"

#define I2C_BUS             (0)
#define I2C_DEVICE_NAME     ("SSD1306FB")
#define SSD1306_ADDR        (0x3C)

// -------------- Functions
static int __init ssd1306_init(void);
static void __exit ssd1306_exit(void);
static int ssd1306_probe(struct i2c_client *client, const struct i2c_device_id *id);
static void ssd1306_remove(struct i2c_client *client);



// -------------- Variables
static struct i2c_client  *i2c_client = NULL;

static const struct i2c_device_id ssd1306_id[] = {
    { I2C_DEVICE_NAME, 0 },
    { }
};
MODULE_DEVICE_TABLE(i2c, ssd1306_id);

static struct i2c_driver ssd1306_driver = {
  .driver = {
    .name   = I2C_DEVICE_NAME,
    .owner  = THIS_MODULE
  },
  .probe          = ssd1306_probe,
  .remove         = ssd1306_remove,
  .id_table       = ssd1306_id,
};

static struct i2c_board_info i2c_info = {
    I2C_BOARD_INFO(I2C_DEVICE_NAME, SSD1306_ADDR)
};

//===================================================
//                  Device file
//===================================================

#include <linux/cdev.h>


dev_t dev = 0;
static struct class *dev_class;
static struct cdev ssd1306_cdev;

// static int ssd1306_open(struct inode *inode, struct file *file);
// static ssize_t ssd1306_write_sc(struct file *filp, const char __user *buf, size_t len, loff_t *off);
static long ssd1306_ioctl(struct file *file, unsigned int cmd, unsigned long arg);

static struct file_operations fops =
{
    .owner      = THIS_MODULE,
    .unlocked_ioctl = ssd1306_ioctl,
    // .open       = ssd1306_open,
    // .write      = ssd1306_write_sc,

};

// static int ssd1306_open(struct inode *inode, struct file *file)
// {
//         pr_info("Driver Open Function Called...!!!\n");
//         SSD1306_String("Open Worked :D\n", i2c_client);
//         return 0;
// }

// static ssize_t ssd1306_write_sc(struct file *filp, const char __user *buf, size_t len, loff_t *off)
// {
//         pr_info("Driver Write Function Called...!!!\n");
//         SSD1306_String("Write Worked :D\n", i2c_client);

//         SSD1306_String(buf, i2c_client);

//         return len;
// }

//===================================================
//                  IOCTL
//===================================================

#define WRITE_STRING _IOW(190982, 'a', char *)
#define CLEAR_DISPLAY _IO(190982, 'c')

// #define RD_VALUE _IOR('a','b',int32_t*)
// #define RD_VALUE2 _IOR('a','d',char *)

int32_t value = 0;
char word[1024];

static long ssd1306_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
    switch(cmd) {
        case WRITE_STRING:
            if( copy_from_user(&word ,(int32_t*) arg, sizeof(word)) )
                pr_err("Data Write : Err!\n");

            pr_info("Value = %d\n", value);
            SSD1306_String(word, i2c_client);
            break;
        case CLEAR_DISPLAY:
            SSD1306_SetCursor(0,0, i2c_client);  
            SSD1306_Fill(0x00, i2c_client);
            SSD1306_command_turnon(SSD1306_DISPLAY_OFF, i2c_client);
            pr_info("Cleared Display");
            break;
        // case RD_VALUE2:
        //     if( copy_to_user((char *) arg, &word, sizeof(word)) )
        //     {
        //             pr_err("Data Read : Err!\n");
        //     }
        //     break;
        default:
            pr_info("Default\n");
            break;
    }
    return 0;
}