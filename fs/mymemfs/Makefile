
obj-m += mymemfs.o 

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	rm *~	

install:
	sudo insmod mymemfs.ko

remove: 
	sudo rmmod mymemfs.ko

info:
	modinfo mymemfs.ko


