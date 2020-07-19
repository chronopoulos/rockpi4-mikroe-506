SRCS = mikroe-506.c wm8731.c

#DTC = dtc   # must be >= 1.6.0
DTC = /home/linaro/software/dtc/dtc   # must be >= 1.6.0
DTC_FLAGS = -@ -Hepapr -I dts -O dtb 
DTS = rockpi-mikroe-506-overlay.dts
DTBO = rockpi-mikroe-506.dtbo 

OVERLAY_DIR = /boot/overlays

obj-m += mikroe-506.o
obj-m += wm8731.o

####

all: drivers overlay

drivers: $(SRCS)
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

overlay: $(DTS)
	$(DTC) $(DTC_FLAGS) -o $(DTBO) $(DTS)

install:
	cp $(DTBO) $(OVERLAY_DIR)
