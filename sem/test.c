
#include <stdio.h>
#include <fcntl.h>

#define BUF_LEN 4096
char buf[BUF_LEN];

int main(int argc, char *argv[])
{
	int ret, count = BUF_LEN, fd;
	if(argc < 2)
	{
		printf("\nUtility to test character device driver wait on semaphore operation upon read\n");
		printf("\nUsage %s <device_file>\n", argv[0]);
		printf("\nExamples:\n\n");
		printf("%s /dev/mydemdev1\n", argv[0]);
		printf("%s /dev/mydemdev2\n", argv[0]);

		printf("\nNote: The device file name must be created before running this program.\n");
		printf("      The device number can be obtained from dmesg\n\n");
		return 1;
	}
	
	fd = open(argv[1], O_RDONLY);
	if(fd < 0)
	{
		perror(argv[1]);
		return 2;
	}
	printf("Requesting %d bytes of data\n", count);
	ret = read(fd, buf, count);
	printf("Obtained %d bytes of data\n", ret);
	printf("\n<<-----Data---->>\n");
	printf("%s", buf);
	printf("\n<<-----End of Data---->>\n");
	printf("Press enter to continue to close the file\n");
	getchar();
	close(fd);
	return 0;
}



