
obj-m += task.o 

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

install:
	sudo insmod task.ko

remove: 
	sudo rmmod task

info:
	modinfo task.ko


