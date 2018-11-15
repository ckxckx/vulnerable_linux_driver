
#ifndef _CKX_NULL_
#define _CKX_NULL_

void ckx_null(void){
void (*fun)(void);
fun = NULL;
//printk("%s:thread.sp0: %p, task->stack: %p\n", "PROC", current->thread.sp0, current->stack);
fun();
}

#endif