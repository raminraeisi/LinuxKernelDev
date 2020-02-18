

#include <linux/module.h>
#include <linux/init.h>

#include <linux/usb.h>


static struct usb_device *device;

#define ML_VENDOR_ID	0x0483
#define ML_PRODUCT_ID	0x5750

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
	struct usb_host_interface *iface_desc;
	struct usb_endpoint_descriptor *endpoint;
	int i;

	iface_desc = interface->cur_altsetting;
	printk(KERN_INFO "Pen i/f %d now probed: (%04X:%04X)\n",
					iface_desc->desc.bInterfaceNumber,
					id->idVendor, id->idProduct);
	printk(KERN_INFO "ID->bNumEndpoints: %02X\n",
					iface_desc->desc.bNumEndpoints);
	printk(KERN_INFO "ID->bInterfaceClass: %02X\n",
					iface_desc->desc.bInterfaceClass);

	for (i = 0; i < iface_desc->desc.bNumEndpoints; i++)
	{
			endpoint = &iface_desc->endpoint[i].desc;

			if ((endpoint->bEndpointAddress & USB_ENDPOINT_DIR_MASK) == USB_DIR_IN)
				printk(KERN_INFO "ED[%d] is input", i);
			else if ((endpoint->bEndpointAddress & USB_ENDPOINT_DIR_MASK) == USB_DIR_OUT)
					printk(KERN_INFO "ED[%d] is output", i);


			printk(KERN_INFO "ED[%d]->bEndpointAddress: 0x%02X\n",
							i, endpoint->bEndpointAddress);
			printk(KERN_INFO "ED[%d]->bmAttributes: 0x%02X\n",
							i, endpoint->bmAttributes);
			printk(KERN_INFO "ED[%d]->wMaxPacketSize: 0x%04X (%d)\n",
							i, endpoint->wMaxPacketSize,
							endpoint->wMaxPacketSize);
	}

	device = interface_to_usbdev(interface);
	return 0;
}

static void drv_disconnect(struct usb_interface *interface)
{
	printk(KERN_INFO "STM32 i/f %d now disconnected\n",
					interface->cur_altsetting->desc.bInterfaceNumber);
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
