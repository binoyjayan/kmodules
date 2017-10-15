
obj-m += proctest.o

proctest-objs +=  procfs.o hook.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

install:
	sudo insmod proctest.ko

remove:
	sudo rmmod proctest
