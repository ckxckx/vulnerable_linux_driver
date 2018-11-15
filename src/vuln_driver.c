#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/miscdevice.h>
#include <linux/slab.h>
#include <linux/ioctl.h>
#include <linux/uaccess.h>
#include<linux/fs_struct.h>

#include <linux/init_task.h>
// #include<sys/types.h>
// #include<unistd.h>

#include "vuln_driver.h"

#include "ckx_escaperoot.h"
#include "ckx_rw.h"
#include "ckx_null.h"
#include "ckx_rootonly.h"






static long do_ioctl(struct file *filp, unsigned int cmd, unsigned long args)
{
	int ret;
	unsigned long *p_arg = (unsigned long *)args;
 	ret = 0;

	switch(cmd) {

		case CKX_ESCAPEROOT:
		{
			
			ckx_escaperoot();
			ret=1233;
			break;
		}
		case CKX_ROOTONLY:
		{

			ckx_rootmeonly();
			ret = 1234;
			break;
		}

		case ARBITRARY_RW_WRITE_USER_CKX:
        {

			ckx_rw(p_arg);
			ret=1235;

            break;
        }
    	case CKX_NULL:
        {

			ckx_null();
			ret = 1236;
            //printk("The memory of %p : %d\n", proc_user, *proc_user);
			break;
        }		
	}

	return ret;
}










static int vuln_release(struct inode *inode, struct file *filp)
{


	return 0;
}

/**
* The operations allowed by userspace applications.
* We only really allow access through the ioctl interface.
*/
static struct file_operations vuln_ops = {
	.owner = THIS_MODULE,
	.unlocked_ioctl = do_ioctl,
	.release = vuln_release,
};

/**
* The miscdevice api is much simpler than creating a class
* and attaching it to a node using class_create etc.
*/
static struct miscdevice vuln_device = {
	MISC_DYNAMIC_MINOR, DEVICE_NAME, &vuln_ops
};

/**
* Register the device.
*/
static int vuln_module_init(void)
{
	int ret;

	ret = misc_register(&vuln_device);

	if(ret < 0) {
		printk(KERN_WARNING "[-] Error registering device [-]\n");
	}

	// printk(KERN_WARNING "[!!!] use_stack_obj @%p [!!!]\n", use_stack_obj);

	return ret;
}

/**
* Deregister the device.
*/
static void vuln_module_exit(void)
{
	misc_deregister(&vuln_device);
}

module_init(vuln_module_init);
module_exit(vuln_module_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Invictus");
MODULE_DESCRIPTION("");
