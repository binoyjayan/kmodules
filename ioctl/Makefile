
obj-m += mymemdev.o 

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

install:
	sudo insmod mymemdev.ko

remove: 
	sudo rmmod mymemdev.ko

info:
	modinfo mymemdev.ko


