PCKERNELDIR=/lib/modules/$(shell uname -r)/build/
KERNELDIR=/home/guo/Downloads/kernel-3.4.39
PWD=$(shell pwd)
all:
	make -C $(KERNELDIR) M=$(PWD) modules
	@echo "build on board"
	arm-linux-gcc led_main.c -o led_main
	cp *.ko led_main ~/rootfs/
# M是个变量
pc:
	make -C $(PCKERNELDIR) M=$(PWD) modules
	@echo "build on pc"
clean:
	make -C $(KERNELDIR) M=$(PWD) clean

obj-m:= led_ctl.o
