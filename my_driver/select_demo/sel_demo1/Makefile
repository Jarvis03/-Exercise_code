PCKERNELDIR=/lib/modules/$(shell uname -r)/build/
KERNELDIR=/home/guo/Downloads/kernel-3.4.39
PWD=$(shell pwd)
all:
	make -C $(PCKERNELDIR) M=$(PWD) modules
	@echo "build on pc"
b:
	make -C $(KERNELDIR) M=$(PWD) modules
	@echo "build on board"
	arm-linux-gcc led_main.c -o led_main
	cp *.ko led_main ~/rootfs/
# M是个变量
clean:
	make -C $(KERNELDIR) M=$(PWD) clean

obj-m:= led_drv1.o
