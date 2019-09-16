PCKERNELDIR=/lib/modules/$(shell uname -r)/build/
KERNELDIR=/home/guo/Downloads/kernel-3.4.39
PWD=$(shell pwd)
all:
	make -C $(KERNELDIR) M=$(PWD) modules
	@echo "build on board"
	arm-linux-gcc adc_main.c -o adc_main
	cp *.ko adc_main ~/rootfs/
# M是个变量
#p:
#	make -C $(PCKERNELDIR) M=$(PWD) modules
#	@echo "build on pc"
clean:
	make -C $(KERNELDIR) M=$(PWD) clean

obj-m:= adc_int.o
