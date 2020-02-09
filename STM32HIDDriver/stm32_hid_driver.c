

#include <linux/module.h>
#include <linux/init.h>

#include <linux/usb.h>

#define ML_VENDOR_ID	0x2123
#define ML_PRODUCT_ID	0x1010

#define DEBUG_LEVEL_DEBUG		0x1F
#define DEBUG_LEVEL_INFO		0x0F
#define DEBUG_LEVEL_WARN		0x07
#define DEBUG_LEVEL_ERROR		0x03
#define DEBUG_LEVEL_CRITICAL	0x01

#define DBG_DEBUG(fmt, args...) \
if ((debug_level & DEBUG_LEVEL_DEBUG) == DEBUG_LEVEL_DEBUG) \
	printk( KERN_DEBUG "[debug] %s(%d): " fmt "\n", \
			__FUNCTION__, __LINE__, ## args)
#define DBG_INFO(fmt, args...) \
if ((debug_level & DEBUG_LEVEL_INFO) == DEBUG_LEVEL_INFO) \
	printk( KERN_DEBUG "[info]  %s(%d): " fmt "\n", \
			__FUNCTION__, __LINE__, ## args)
#define DBG_WARN(fmt, args...) \
if ((debug_level & DEBUG_LEVEL_WARN) == DEBUG_LEVEL_WARN) \
	printk( KERN_DEBUG "[warn]  %s(%d): " fmt "\n", \
			__FUNCTION__, __LINE__, ## args)
#define DBG_ERR(fmt, args...) \
if ((debug_level & DEBUG_LEVEL_ERROR) == DEBUG_LEVEL_ERROR) \
	printk( KERN_DEBUG "[err]   %s(%d): " fmt "\n", \
			__FUNCTION__, __LINE__, ## args)
#define DBG_CRIT(fmt, args...) \
if ((debug_level & DEBUG_LEVEL_CRITICAL) == DEBUG_LEVEL_CRITICAL) \
	printk( KERN_DEBUG "[crit]  %s(%d): " fmt "\n", \
			__FUNCTION__, __LINE__, ## args)

static int debug_level = DEBUG_LEVEL_INFO;
static int debug_trace = 0;

module_param(debug_level, int, S_IRUGO | S_IWUSR);
module_param(debug_trace, int, S_IRUGO | S_IWUSR);
MODULE_PARM_DESC(debug_level, "debug level (bitmask)");
MODULE_PARM_DESC(debug_trace, "enable function tracing");

static struct usb_device_id usb_id_table [] = {
	{ USB_DEVICE(ML_VENDOR_ID, ML_PRODUCT_ID) },
	{ }
};

static int drv_probe(struct usb_interface *interface,
		    const struct usb_device_id *id)
{
}

static void drv_disconnect(struct usb_interface *interface)
{
}

static struct usb_driver driver_st = {
	.name = "stm32_hid_driver",
	.id_table = usb_id_table,
	.probe = drv_probe,
	.disconnect = drv_disconnect,
};

static int __init drv_stm32_hid_init(void)
{
	int result;

	DBG_INFO("Register driver");
	result = usb_register(&driver_st);
	if (result) {
		DBG_ERR("registering driver failed");
	} else {
		DBG_INFO("driver registered successfully");
	}

	return result;
}


static void __exit drv_stm32_hid_exit(void)
{
	usb_deregister(&driver_st);
	DBG_INFO("module deregistered");
}

module_init(drv_stm32_hid_init);
module_exit(drv_stm32_hid_exit);

MODULE_AUTHOR("Ramin Raeisi");
MODULE_LICENSE("GPL");

// vim: ts=4
