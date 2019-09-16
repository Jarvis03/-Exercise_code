PCKERNELDIR=/lib/modules/$(shell uname -r)/build/
KERNELDIR=/home/guo/Downloads/kernel-3.4.39
PWD=$(shell pwd)
all:
	make -C $(PCKERNELDIR) M=$(PWD) modules
	@echo "build on pc"
b:
	make -C $(KERNELDIR) M=$(PWD) modules
	@echo "build on board"
	cp *.ko  ~/rootfs/
# M是个变量
clean:
	make -C $(KERNELDIR) M=$(PWD) clean
obj-m:= adc_drv.o
