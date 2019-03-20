TARGET_MOUDLE := simple_driver
obj-m += $(TARGET_MOUDLE).o
KERNEL_SRC := /lib/modules/$(shell uname -r )/build
SRC := $(shell pwd)

all:
	make -C $(KERNEL_SRC) M=$(SRC) modules
clean:
	make -C $(KERNEL_SRC) M=$(SRC) clear
load:
	insmod ./$(TARGET_MOUDLE).ko
unload:
	rmmod ./$(TARGET_MOUDLE).ko
