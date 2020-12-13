SRCS = mikroe-506.c wm8731.c
obj-m += mikroe-506.o
obj-m += wm8731.o

KERNEL_DIR = /lib/modules/$(shell uname -r)
KERNEL_BUILD_DIR = $(KERNEL_DIR)/build
KERNEL_KO_DIR = $(KERNEL_DIR)/kernel/drivers

##

DTC = dtc   # must be >= 1.6.0
DTC_FLAGS = -@ -Hepapr -I dts -O dtb 
DTS = rockpi-mikroe-506-overlay.dts
DTBO = rockpi-mikroe-506.dtbo 

OVERLAY_DIR = /boot/overlays

####

all: drivers overlay

drivers: $(SRCS)
	make -C $(KERNEL_BUILD_DIR) M=$(PWD) modules

overlay: $(DTS)
	$(DTC) $(DTC_FLAGS) -o $(DTBO) $(DTS)

install:
	cp $(SRCS:c=ko) $(KERNEL_KO_DIR)
	cp $(DTBO) $(OVERLAY_DIR)
	depmod -a

clean:
	make -C $(KERNEL_BUILD_DIR) M=$(PWD) clean
	rm -f $(DTBO)
