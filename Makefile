
obj-m += ssd1306_out.o
ssd1306_out-objs := ssd1306.o ssd1306_util.o  ssd1306_commands.o

KERNELDIR = /home/joao/Documents/2.Linux/linux-mainline-6.1.25

PWD := $(shell pwd)

all:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) clean