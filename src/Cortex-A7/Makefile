# ============================================================================================
# 
#  Author:       Jonathan Edwards
#  Date:         20/11/2022
# 
#  Description:  
# 				This makefile controls $(TARGET_2).mk, Detection.mk, and 
# 					$(TARGET_1).mk to build and run the project.  
# 				This file also allows the user specify the location of the 
# 					stm32mp157 environment variables. 
# 
#  ============================================================================================

# https://www.tomshardware.com/how-to/monitor-cpu-and-ram-in-linux
# >> grep MemTotal /proc/meminfo -- checking how much ram you have
# >> free -m - check how much ram u have currently
# >> cat /proc/cpuinfo - cpuinfo

# sudo cp usr/bin/* /media/jonathan/rootfs/usr/bin/

# App Name
PROJ_NAME = Imaging

# 0 for x86_64, 1 for armv7 32-bit architecture
ARM		= 1
# 0 for debug, 1 for No debug
DEBUG	= 1
# Home path of stm32mp1 sysroots directory 
HOME = /home/jonathan/Desktop/Embedded_Systems/Software/apps/LUCY-II-yocto-odyssey
# Path to project directory 
PATH_PROJ_DIR = /home/jonathan/Desktop/Embedded_Systems

# Ethernet configs
SSH_IPv4 = 169.254.128.28
SET_ETH_0 = ifconfig eth0 $(SSH_IPv4) broadcast 169.254.255.255 netmask 255.255.0.0

# Environment setup for cross compiling to the stm32mp157
SETUP = . $(HOME)/SDK/environment-setup-cortexa7t2hf-neon-vfpv4-poky-linux-gnueabi;
ifeq ($(ARM), 0)
	SETUP = 
endif

# Definitions
CC 				= arm-poky-linux-gnueabi-gcc
CXX 			= arm-poky-linux-gnueabi-g++
ARCH_FLAGS		= -mfpu=neon-vfpv4 -mfloat-abi=hard -mcpu=cortex-a7 -fstack-protector-strong -D_FORTIFY_SOURCE=2 -Wformat -Wformat-security -Werror=format-security --sysroot=$(HOME)/SDK/sysroots/cortexa7t2hf-neon-vfpv4-poky-linux-gnueabi
OPT 			= -O3
ifeq ($(ARM), 0)
	CC				= gcc
	CXX				= g++
	ARCH_FLAGS		= -m64
endif
ifeq ($(DEBUG), 0)
	OPT 			= -g3 -Og
endif
CFLAGS   		= $(OPT) -Wall -std=c11 $(ARCH_FLAGS) -fPIC
CXXFLAGS   		= $(OPT) -Wall -std=c++11 $(ARCH_FLAGS) -fPIC

# Libraries & Includes
# -I/ (represents the systems directory)
# -I./ (represents the current workspace directory)
INCLUDE_OPENCV = -I$(HOME)/SDK/sysroots/cortexa7t2hf-neon-vfpv4-poky-linux-gnueabi/usr/include/opencv4/opencv \
			 	 -I$(HOME)/SDK/sysroots/cortexa7t2hf-neon-vfpv4-poky-linux-gnueabi/usr/include/opencv4
INCLUDE_VIMBAC = -I/usr/lib
# use `ldd ./xxx` to find the shared libraries used
LIBS_1     	= -lvimba_c -lopencv_core -lopencv_imgproc -lopencv_imgcodecs  
LIBS_2		= 
LIBS_3     	= -lopencv_core -lopencv_imgproc -lopencv_imgcodecs 
LIBS_4		= 
ifeq ($(ARM), 0)
	INCLUDE_OPENCV = -I/usr/include/opencv4/opencv -I/usr/include/opencv4
endif

INCLUDES_1 = -I./$(TARGET_1)/inc/ $(INCLUDE_VIMBAC) $(INCLUDE_OPENCV)
INCLUDES_2 = -I./$(TARGET_2)/inc/
INCLUDES_3 = -I./$(TARGET_3)/inc/ $(INCLUDE_OPENCV)
INCLUDES_4 = -I./$(TARGET_4)/inc/


# Pretty Printing
PRINT_BEGIN      = "======================================================================================================================"
PRINT_END        = "======================================================================================================================\n\n"
REMOVE_MESSAGE   = "-------------------------------------- Removing all object and executable files --------------------------------------"
INIT_PRJ_MESSAGE = "--------------------------------------- Initialising project files and folders ---------------------------------------"
LINK_NAN_MESSAGE = "------------------------------------------ Creating non linked object files ------------------------------------------"
LINK_OBJ_MESSAGE = "-------------------------------- Linkng all object files to create an executable file --------------------------------"



# Default target
all: clean init $(PROJ_NAME)
	@echo Project applications successfully built...


# Building object files
include CaptureImage/CaptureImage.mk
include CrystalDetection/CrystalDetection.mk
include Autofocus/Autofocus.mk
include SerialCommunication/SerialCommunication.mk


# Link: Create output file from object files
$(PROJ_NAME): $(OBJECTS) $(OBJECTS_COM) $(OBJECTS_DET) $(OBJECTS_AUTO)
	@echo $(PRINT_END)
	@echo $(LINK_OBJ_MESSAGE)
	@echo $(PRINT_BEGIN)
	$(SETUP) \
	$(CXX) $(ARCH_FLAGS) $(TARGET_1)/build/*.o -o $(TARGET_1)/build/$(TARGET_1l) $(LIBS_1)
	$(SETUP) \
	$(CXX) $(ARCH_FLAGS) $(TARGET_3)/build/*.o -o $(TARGET_3)/build/$(TARGET_3l) $(LIBS_3)
	$(SETUP) \
	$(CC) $(ARCH_FLAGS) $(TARGET_4)/build/*.o -o $(TARGET_4)/build/$(TARGET_4l) $(LIBS_4)
	gcc -m64 $(TARGET_2)/build/*.o -o $(TARGET_2)/build/$(TARGET_2l) $(LIBS_2)
	size $(TARGET_1)/build/$(TARGET_1l)
	size $(TARGET_3)/build/$(TARGET_3l)
	size $(TARGET_4)/build/$(TARGET_4l)
	size $(TARGET_2)/build/$(TARGET_2l)
	@echo
	cp $(TARGET_1)/build/$(TARGET_1l) usr/bin/
	cp usr/bin/$(TARGET_1l) $(HOME)/meta-custom/recipes-apps/$(TARGET_1)/files/bin/
	cp usr/bin/$(TARGET_1l).sh $(HOME)/meta-custom/recipes-apps/$(TARGET_1)/files/bin/
	cp $(TARGET_3)/build/$(TARGET_3l) usr/bin/
	cp usr/bin/$(TARGET_3l) $(HOME)/meta-custom/recipes-apps/$(TARGET_3)/files/bin/
	cp $(TARGET_4)/build/$(TARGET_4l) usr/bin/
	cp usr/bin/$(TARGET_4l) $(HOME)/meta-custom/recipes-apps/$(TARGET_4)/files/bin/
	cp usr/bin/controller_A7.sh $(HOME)/meta-custom/recipes-apps/Controller/files/bin/
	@echo $(PRINT_END)


# Target: Init Project.
.PHONY: init
init:
	@echo $(INIT_PRJ_MESSAGE)
	@echo $(PRINT_BEGIN)
	-mkdir scripts/log/
	-mkdir images/
	-mkdir $(TARGET_1)/build/
	-mkdir $(TARGET_2)/build/
	-mkdir $(TARGET_3)/build/
	-mkdir $(TARGET_4)/build/
	@echo $(PRINT_END)
	@echo $(LINK_NAN_MESSAGE)
	@echo $(PRINT_BEGIN)


# Target: Clean Project.
.PHONY: clean
clean:
	@echo $(REMOVE_MESSAGE)
	@echo $(PRINT_BEGIN)
	rm -f -r log/
	rm -f -r scripts/log/
	rm -f -r images/
	rm -f -r $(TARGET_1)/build/
	rm -f -r $(TARGET_2)/build/
	rm -f -r $(TARGET_3)/build/
	rm -f -r $(TARGET_4)/build/
	rm -f usr/bin/$(TARGET_1l)
	rm -f usr/bin/$(TARGET_3l)
	rm -f usr/bin/$(TARGET_4l)
	@echo $(PRINT_END)


# ./capture_image settings/camera_settings_color.xml
RUN = rm -r -f images/*
RUN += && sh scripts/ground_controller.sh $(SSH_IPv4)
ifeq ($(ARM), 0)
	RUN = $(TARGET_1)/build/$(TARGET_1l)
	RUN += && $(TARGET_3)/build/$(TARGET_3l)
	RUN += && mv *.bmp images/
endif
.PHONY: program
program: 
	ssh -t root@$(SSH_IPv4) 'cd /usr/bin/ && \
	sh controller_A7.sh $(TARGET); \
	exit;\
	exec /bin/sh --login'; exec bash; 


# Login: root
monitor_uart: 
	sh scripts/monitor.sh

# sh stm32_usbotg_eth_config.sh restart
monitor_usb0: 
	ssh -t root@$(SSH_IPv4) 'cd /usr/bin/; exec /bin/sh --login'; exec bash; 
# gnome-terminal -- bash -c "ssh root@$(SSH_IPv4) -t 'cd / && exec /bin/sh --login'; exec bash"

# Send updated files to the client machine
send:
	scp -r usr/* root@$(SSH_IPv4):/usr/
	
receive:
	scp -r root@$(SSH_IPv4):/usr/etc/images/ ./

CHECK_RUN = $(TARGET_1)/build/$(TARGET_1) settings/camera_settings_color.xml
# CHECK_RUN = $(TARGET_2)/build/$(TARGET_2) -C -t 200

memory_check:
	rm -f -r log/
	-mkdir log/
	echo "New log..." > scripts/log/monitor.log
	valgrind --log-file=log/memory_check.log --leak-check=full $(CHECK_RUN)
	-mv *.bmp images/

copy_vimba:
	sudo cp $(PATH_PROJ_DIR)/Software/libs/Vimba_4_0-armv7/VimbaC/DynamicLib/arm_32bit/libvimba_c.so $(HOME)/yocto/SDK/sysroots/cortexa7t2hf-neon-vfpv4-poky-linux-gnueabi/usr/lib/
	sudo cp $(PATH_PROJ_DIR)/Software/libs/Vimba_6_0-x86_64/VimbaC/DynamicLib/x86_64bit/libvimba_c.so /usr/lib/

