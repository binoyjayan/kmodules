/*
 * Demonstrating a character device driver which can 
 * read/write from a memory device
 * Author: Binoy Jayan
 * Date Written:Monday, September 16th, 2013
*/

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/string.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <asm/uaccess.h>
#include <linux/errno.h>

#define CHARDEV_AUTHOR	"Binoy Jayan[binoyjayan@gmail.com]"
#define CHARDEV_LICENSE	"GPL"
#define CHARDEV_DESC	"A Simple memory based character device"
#define CHARDEV_DEVICE	"mymemdev"

MODULE_LICENSE(CHARDEV_LICENSE);
MODULE_AUTHOR(CHARDEV_AUTHOR);
MODULE_DESCRIPTION(CHARDEV_DESC);
MODULE_SUPPORTED_DEVICE(CHARDEV_DEVICE);

#define MYMEMDEV_NAME  "mymemdev"
#define MYMEMDEV_SIZE  10 * 1024 * 1024

static int mymemdev_major = 0;
static int mymemdev_minor = 1;
static int mymemdev_nr_devs = 1;

struct mymemdev_device_t
{
	const char *name;
	char data[MYMEMDEV_SIZE];
	unsigned long size;
	unsigned long maxsize;
	//struct mutex m;
	struct cdev cdev;	
};

struct mymemdev_device_t mymemdev = {
	.name 	 = MYMEMDEV_NAME,
	.maxsize = MYMEMDEV_SIZE,
};

struct mymemdev_device_t *mymemdev_get_device(struct cdev *cdev)
{
	return container_of(cdev, struct mymemdev_device_t, cdev);
}

int mymemdev_truncate(struct mymemdev_device_t *dev)
{
	//TODO:Should I clear the data upon a file truncate?
	//memset(dev->data, 0, sizeof(dev->data));//?
	dev->size = 0;
	return 0;
}

int mymemdev_open(struct inode *inode, struct file *filp)
{
	struct mymemdev_device_t *dev;
	dev = mymemdev_get_device(inode->i_cdev);
	filp->private_data = dev;
	if ((filp->f_flags & O_ACCMODE) == O_WRONLY)
		mymemdev_truncate(dev);
	return 0;
}

int mymemdev_release(struct inode *inode, struct file *filp)
{
	return 0;
}

ssize_t mymemdev_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos)
{
	ssize_t ret = 0;
	struct mymemdev_device_t *dev = (struct mymemdev_device_t *) filp->private_data;
	if (*f_pos >= dev->size)
	{
		printk("%s:read:requested %d bytes. File position %ld\n", dev->name, count, (long)*f_pos);
		goto out;
	}

	if (*f_pos + count > dev->size)
	{
		count = dev->size - *f_pos;
	}

	printk("%s:read:returning %d bytes\n", dev->name, count);

	if (copy_to_user(buf, dev->data + *f_pos, count)) {
		ret = -EFAULT;
		goto out;
	}
	*f_pos += count;
	ret = count;
out:
	return ret;
}

ssize_t mymemdev_write(struct file *filp, const char __user *buf, size_t count, loff_t *f_pos)
{
	ssize_t ret = -ENOMEM;
	struct mymemdev_device_t *dev = (struct mymemdev_device_t *) filp->private_data;
	if (*f_pos >= dev->maxsize)
	{
		printk("%s:write:No space left. file pos:%ld\n", dev->name, (long)*f_pos);
		goto out;
	}

	if (count > dev->maxsize - *f_pos)
		count = dev->maxsize - *f_pos;

	if (copy_from_user(dev->data + dev->size, buf, count)) {
		ret = -EFAULT;
		goto out;
	}
	*f_pos += count;
	if (dev->size < *f_pos)
		dev->size = *f_pos;
	printk("%s:write:written %d bytes. New file pos:%ld\n", dev->name, count, (long)*f_pos);
	ret = count;

out:
	return ret;
}

struct file_operations mymemdev_fops = {
	.owner = THIS_MODULE,
	.read = mymemdev_read,
	.write = mymemdev_write,
	.open = mymemdev_open,
	.release = mymemdev_release
};

int mymemdev_register(struct mymemdev_device_t *dev)
{
	dev_t devnum;
	int res, index;
	if (mymemdev_major)
	{
		devnum = MKDEV(mymemdev_major, mymemdev_minor);
		res = register_chrdev_region(devnum, mymemdev_nr_devs, dev->name);
	} else {
		res = alloc_chrdev_region(&devnum, mymemdev_minor, mymemdev_nr_devs, dev->name);
		mymemdev_major = MAJOR(devnum);
	}
	if (res < 0) {
		printk(KERN_WARNING "%s: cannot get major number %d\n", dev->name, mymemdev_major);
		goto out;
	}
	index = 1;
	cdev_init(&dev->cdev, &mymemdev_fops);
	dev->cdev.owner = THIS_MODULE;
	dev->cdev.ops = &mymemdev_fops;
	res = cdev_add (&dev->cdev, devnum, index);
	if (res)
		printk(KERN_WARNING "%s:Error %d adding mymemdev%d", dev->name, res, index);

out:
	return res;
}

void mymemdev_unregister(struct mymemdev_device_t *dev)
{
	dev_t devnum;
	devnum = MKDEV(mymemdev_major, mymemdev_minor);
	unregister_chrdev_region(devnum, mymemdev_nr_devs);
	printk(KERN_NOTICE "%s:Unregistered memory device\n", dev->name);
}

static int __init mymemdev_init(void)
{
	int res;
	printk(KERN_INFO "%s: A Simple memory Character Device Driver\n", mymemdev.name);
	printk(KERN_INFO "-----------------------------------------------------------\n");
	
	res = mymemdev_register(&mymemdev);
	if(res < 0)
	{
		printk(KERN_ALERT "%s:Registering chardev failed with %d\n", mymemdev.name, res);
		return res;
	}
	printk(KERN_INFO "%s:Assigned device number : (%d, %d)\n", mymemdev.name, mymemdev_major, mymemdev_minor);
	printk(KERN_INFO "%s:Create a device with   :\n", mymemdev.name);
	printk(KERN_INFO "         sudo mknod /dev/%s c %d %d\n", mymemdev.name, mymemdev_major, mymemdev_minor);
	printk(KERN_INFO "         sudo chmod a+w /dev/%s\n", mymemdev.name);
	printk(KERN_INFO "-----------------------------------------------------------\n");
	return 0;
}

static void __exit mymemdev_exit(void)
{
	mymemdev_unregister(&mymemdev);
	printk(KERN_ALERT "%s:Unregistered memory character device.\n", mymemdev.name);
}

module_init(mymemdev_init);
module_exit(mymemdev_exit);

