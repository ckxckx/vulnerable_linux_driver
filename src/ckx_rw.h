#ifndef _CKX_RW_
#define _CKX_RW_

#include "ckx_structs.h"

void ckx_rw(unsigned long *p_arg){
    write_args_ckx w_args_ckx;
    char ckxbuffer[0x500];
    unsigned long count;
    void *ckxto;
    void *ckxsrc;
    printk("[*] ckx: action1\n");
    if (copy_from_user(&w_args_ckx, p_arg, sizeof(write_args_ckx)))
        return -EINVAL;

    // ret = write_mem_buffer(&w_args);

    ckxto = w_args_ckx.to;
    ckxsrc = w_args_ckx.buff;
    count = w_args_ckx.count;
    printk("ckxto is %p\n", ckxto);
    printk("ckxSRC is %p\n", ckxsrc);
    printk("COUNT is %ld\n", count);

    printk("[*] ckx: action2\n");
    if (copy_from_user(ckxto, ckxsrc, 2))
        ;
    return -EINVAL;

    printk("[*] ckx: action3\n");
    // printk("[*] target is:%p\n",ckxto);
    // printk ("[*] target is padding : %s\n",ckxto);
}

#endif