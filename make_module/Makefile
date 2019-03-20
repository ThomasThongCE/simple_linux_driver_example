obj-m += simple_driver.o
KERNEL_SRC := /lib/modules/$(shell uname -r )/build
SRC := $(shell pwd)

all:
	make -C $(KERNEL_SRC) M=$(SRC) modules
clean:
	make -C $(KERNEL_SRC) M=$(SRC) clear
modules_install:
	make -C $(KERNEL_SRC) M=$(SRC) modules_install
