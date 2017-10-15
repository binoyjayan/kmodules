
#include <linux/fs.h>
#include <linux/pagemap.h>
#include <linux/highmem.h>
#include <linux/time.h>
#include <linux/init.h>
#include <linux/string.h>
#include <linux/sched.h>
#include <linux/parser.h>
#include <linux/magic.h>
#include <linux/slab.h>
#include <asm/uaccess.h>

struct mymemfs_mount_opts {
	umode_t mode;
};

enum {
	Opt_mode,
	Opt_err
};

static const match_table_t tokens = {
	{Opt_mode, "mode=%o"},
	{Opt_err, NULL}
};

struct mymemfs_fs_info {
	struct mymemfs_mount_opts mount_opts;
};

int mymemfs_fill_super(struct super_block *sb, void *data, int silent)
{
	struct mymem_fs_info *fsi;
	struct inode *inode;
	int err;

	save_mount_options(sb, data);

	fsi = kzalloc(sizeof(struct mymemfs_fs_info), GFP_KERNEL);
	sb->s_fs_info = fsi;

	if (!fsi)
		return -ENOMEM;
	err = ramfs_parse_options(data, &fsi->mount_opts);
	if (err)
		return err;

	sb->s_maxbytes		= MAX_LFS_FILESIZE;
	sb->s_blocksize		= PAGE_CACHE_SIZE;
	sb->s_blocksize_bits	= PAGE_CACHE_SHIFT;
	sb->s_magic		= RAMFS_MAGIC;
	sb->s_op		= &ramfs_ops;
	sb->s_time_gran		= 1;

	inode = ramfs_get_inode(sb, NULL, S_IFDIR | fsi->mount_opts.mode, 0);
	sb->s_root = d_make_root(inode);
	if (!sb->s_root)
		return -ENOMEM;

	return 0;
}

struct dentry *mymemfs_mount(struct file_system_type *fs_type,
	int flags, const char *dev_name, void *data)
{
	return mount_nodev(fs_type, flags, data, mymemfs_fill_super);
}

static struct dentry *rootfs_mount(struct file_system_type *fs_type,
	int flags, const char *dev_name, void *data)
{
	return mount_nodev(fs_type, flags|MS_NOUSER, data, mymemfs_fill_super);
}

static void mymemfs_kill_sb(struct super_block *sb)
{
	kfree(sb->s_fs_info);
	kill_litter_super(sb);
}

static struct file_system_type mymemfs_fs_type = {
	.name		= "mymemfs",
	.mount		= mymemfs_mount,
	.kill_sb	= mymemfs_kill_sb,
	.fs_flags	= FS_USERNS_MOUNT,
};

static struct file_system_type rootfs_fs_type = {
	.name		= "rootfs",
	.mount		= rootfs_mount,
	.kill_sb	= kill_litter_super,
};

static int __init init_mymemfs_fs(void)
{
	printk("Registering filesystem mymemfs...\n");
	return register_filesystem(&mymemfs_fs_type);
}

module_init(init_mymemfs_fs)

static void __exit exit_mymemfs_fs(void)
{
	printk("De-registering filesystem mymemfs...\n");
	unregister_filesystem(&mymemfs_fs_type);
}
module_exit(exit_mymemfs_fs)

int __init init_rootfs(void)
{
	int err;

	err = register_filesystem(&rootfs_fs_type);

	return err;
}

int __exit exit_rootfs(void)
{
	int err;

	err = unregister_filesystem(&rootfs_fs_type);

	return err;
}



