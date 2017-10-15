

/*
 * User space program to send ioctl commands to the 
 * memory based character driver
 * 
 * Author       : Binoy Jayan[binoy.jayan@wipro.com]
 * Date Written : September 16th, 2013 
*/

#include <stdio.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include "mymemdev.h"


int main(int argc, char *argv[])
{
	unsigned long device_size;
	int fd, op, ret = 0;
	if(argc < 3)
	{
		printf("\nUtility to control memory character device operations\n");
		printf("\nUsage %s <device_file> <operation> [value]\n", argv[0]);
		printf("\nOperations:\n\n");
		printf("0 : Reset    device size. No argument.\n");
		printf("1 : Set      device size. Needs argument\n");
		printf("2 : Get      device size. No argument\n");
		printf("3 : Tell     device size. Needs argument\n");
		printf("4 : Query    device size. No argument\n");
		printf("5 : eXchange device size. Needs argument (Get + Set)\n");
		printf("6 : sHift    device size. Needs argument (Tell + Query)\n");
		printf("\nExamples:\n\n");
		printf("     %s /dev/mymemdev1 0\n", argv[0]);
		printf("sudo %s /dev/mymemdev1 1 512\n", argv[0]);
		printf("     %s /dev/mymemdev1 2\n", argv[0]);
		printf("sudo %s /dev/mymemdev1 3 512\n", argv[0]);
		printf("     %s /dev/mymemdev1 4\n", argv[0]);
		printf("sudo %s /dev/mymemdev1 5 256\n", argv[0]);
		printf("sudo %s /dev/mymemdev1 6 128\n", argv[0]);
		printf("\n");
		return 1;
	}
	
	fd = open(argv[1], O_RDONLY);
	if(fd < 0)
	{
		perror(argv[1]);
		return 2;
	}
	op = atoi(argv[2]);
	if(op < 0 || op > 6)
	{
		printf("%s:Invalid operation %s\n", argv[0], argv[2]);
		return 3;
	}
	if(argc >= 4)
	{
		device_size = (unsigned long) atol(argv[3]);	
	}
	switch(op)
	{
	case 0:
		ret = ioctl(fd, MYMEMDEV_IOCRESET);
		break;
	case 1:
		ret = ioctl(fd, MYMEMDEV_IOCSMAXSIZE, &device_size);//set
		break;
	case 2:
		ret = ioctl(fd, MYMEMDEV_IOCGMAXSIZE, &device_size);//get
		if(ret >= 0)
			printf("Device size = %d\n", device_size);
		break;
	case 3:
		ret = ioctl(fd, MYMEMDEV_IOCTMAXSIZE, &device_size);//tell
		break;
	case 4:
		ret = device_size = ioctl(fd,MYMEMDEV_IOCQMAXSIZE);//query
		if(ret >= 0)
			printf("Device size = %d\n", device_size);
		break;
	case 5:
		ret = ioctl(fd, MYMEMDEV_IOCXMAXSIZE, &device_size); //eXchange
		printf("Old device size = %d\n", device_size);
		break;
	case 6:
		ret = device_size = ioctl(fd, MYMEMDEV_IOCHMAXSIZE, device_size);//sHift
		if(ret >= 0)
			printf("Old device size = %d\n", device_size);
	}
	if(ret < 0)
	{
		perror(argv[1]);
	}
	return 0;
}



