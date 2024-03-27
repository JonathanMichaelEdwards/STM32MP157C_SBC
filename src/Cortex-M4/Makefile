# ============================================================================================
# 
#  Author:       Jonathan Edwards
#  Date:         15/11/2022
# 
#  Description:  Compiles and links all object files to create an output 
#                app for Linux.
# 
#  ============================================================================================



# Application Name
TARGET = Control_Monitor.elf



# Definitions
CC 				= arm-none-eabi-gcc
CFLAGS			= -mcpu=cortex-m4 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32MP157Cxx -DCORE_CM4
CFLAGS		   += -Os -ffunction-sections -fdata-sections -Wall -fstack-usage

INCLUDES 	= -I./src/ -I./inc/ -I./lib/ -I./drivers/ -I./drivers/BSP/ -I./drivers/CMSIS/ -I./drivers/HAL/

# Generating Object Name
SRC = $(wildcard src/*.c)
OBJECTS_SRC = $(patsubst src/%.c, %.o, $(SRC))

SRC_A = $(wildcard src/*.s)
OBJECTS_SRC_A = $(patsubst src/%.s, %.o, $(SRC_A))

SRC_LIB = $(wildcard lib/*.c)
OBJECTS_LIB = $(patsubst lib/%.c, %.o, $(SRC_LIB))

SRC_DRIVERS = $(wildcard drivers/*.c)
OBJECTS_DRIVERS = $(patsubst drivers/%.c, %.o, $(SRC_DRIVERS))

SRC_BSP = $(wildcard drivers/BSP/*.c)
OBJECTS_DRIVERS_BSP = $(patsubst drivers/BSP/%.c, %.o, $(SRC_BSP))

SRC_CMSIS = $(wildcard drivers/CMSIS/*.c)
OBJECTS_DRIVERS_CMSIS = $(patsubst drivers/CMSIS/%.c, %.o, $(SRC_CMSIS))

SRC_HAL = $(wildcard drivers/HAL/*.c)
OBJECTS_DRIVERS_HAL = $(patsubst drivers/HAL/%.c, %.o, $(SRC_HAL))

OBJECTS = $(OBJECTS_SRC) $(OBJECTS_SRC_A) $(OBJECTS_LIB) $(OBJECTS_DRIVERS) $(OBJECTS_DRIVERS_BSP) $(OBJECTS_DRIVERS_CMSIS) $(OBJECTS_DRIVERS_HAL)


# Pretty Printing
PRINT_BEGIN      = "======================================================================================================================"
PRINT_END        = "======================================================================================================================\n\n"
REMOVE_MESSAGE   = "-------------------------------------- Removing all object and executable files --------------------------------------"
LINK_NA_OBJ_MESSAGE = "------------------------------------------ Creating non linked object files ------------------------------------------"
LINK_OBJ_MESSAGE = "-------------------------------- Linkng all object files to create an executable file --------------------------------"
INIT_MESSAGE 	 = "------------------------------------------------------- Set up -------------------------------------------------------"
COMPLETE_MESSAGE  = Succesfully built $(TARGET) for the Coretex-M4



# Default target
all: init $(TARGET)
	@echo $(COMPLETE_MESSAGE)


# Compile: create object files from C source files
%.o: drivers/%.c
	$(CC) -c $(CFLAGS) $(INCLUDES) -MMD -MP -MF./$(@:%.o=build/%.d) -MT./$@ --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb $< -o build/$@

%.o: drivers/BSP/%.c
	$(CC) -c $(CFLAGS) $(INCLUDES) -MMD -MP -MF./$(@:%.o=build/%.d) -MT./$@ --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb $< -o build/$@

%.o: drivers/CMSIS/%.c
	$(CC) -c $(CFLAGS) $(INCLUDES) -MMD -MP -MF./$(@:%.o=build/%.d) -MT./$@ --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb $< -o build/$@

%.o: drivers/HAL/%.c
	$(CC) -c $(CFLAGS) $(INCLUDES) -MMD -MP -MF./$(@:%.o=build/%.d) -MT./$@ --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb $< -o build/$@

%.o: lib/%.c
	$(CC) -c $(CFLAGS) $(INCLUDES) -MMD -MP -MF./$(@:%.o=build/%.d) -MT./$@ --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb $< -o build/$@

%.o: src/%.c
	$(CC) -c $(CFLAGS) $(INCLUDES) -MMD -MP -MF./$(@:%.o=build/%.d) -MT./$@ --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb $< -o build/$@

%.o: src/%.s
	$(CC) -c $(CFLAGS) -x assembler-with-cpp  $(INCLUDES) -MMD -MP -MF./$(@:%.o=build/%.d) -MT./$@ --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb $< -o build/$@



# Link: create output file from object files
$(TARGET): $(OBJECTS)
	@echo Finished building non linked objects
	@echo $(PRINT_END)

	@echo $(LINK_OBJ_MESSAGE)
	@echo $(PRINT_BEGIN)
	
	arm-none-eabi-gcc -mcpu=cortex-m4 -T./STM32MP157CAAX_RAM.ld --specs=nosys.specs -Wl,-Map=build/main.map -Wl,--gc-sections -static --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -Wl,--start-group -u _printf_float -lc -lm -Wl,--end-group build/*.o -o $@

	@echo
	@echo Finished building target: $@
	@echo
	arm-none-eabi-objdump -h -S  $@  > build/main.list
	arm-none-eabi-size $@
	mv $@ usr/bin/
	@echo $(PRINT_END)


# Target: Init Project.
.PHONY: init
init:
	@echo $(INIT_MESSAGE)
	@echo $(PRINT_BEGIN)
	mkdir build
	@echo $(PRINT_END)
	@echo $(LINK_NA_OBJ_MESSAGE)
	@echo $(PRINT_BEGIN)


# Target: Clean Project.
.PHONY: clean
clean:
	@echo $(REMOVE_MESSAGE)
	@echo $(PRINT_BEGIN)
	-rm -r build/ $(TARGET)
	@echo $(PRINT_END)



# USB ethernet configs
# - HOST >> set USB Ethernet ipv4 to Link-Only
# - STM32 >> ifconfig usb <IPv4> 
SSH_IPv4 = 169.254.128.28
SET_ETH_0 = ifconfig usb0 $(SSH_IPv4) broadcast 169.254.255.255 netmask 255.255.0.0


# program: 
# 	scp $(TARGET) root@$(SSH_IPv4):/lib/firmware/
# 	ssh -t root@$(SSH_IPv4) 'cd /lib/firmware/ && \
# 				echo stop > /sys/class/remoteproc/remoteproc0/state; \
# 				echo $(TARGET) > /sys/class/remoteproc/remoteproc0/firmware; \
# 				echo start > /sys/class/remoteproc/remoteproc0/state; \
# 				exit; \
# 				exec /bin/sh --login'; exec bash; 

## RUN imaging
# scp -r root@169.254.128.28:/usr/etc/images/ ./a
# scp root@169.254.128.28:/usr/etc/images/result_mono_2.bmp ./
# scp /usr/etc/images/result_mono_2.bmp jonathan@169.254.7.90:/home/jonathan/Desktop/Embedded_Systems/Software/test-apps
.PHONY: program
program: 
	scp -r usr/ root@$(SSH_IPv4):/
	ssh -t root@$(SSH_IPv4) 'cd /usr/bin/ && \
				sh controller_M4.sh $(TARGET); \
				exit;\
				exec /bin/sh --login'; exec bash; 

monitor_uart: 
	sh scripts/monitor.sh
# gnome-terminal -- bash -c "minicom -D /dev/ttyACM0' cd /usr/local/ && exec /bin/sh --login'; exec bash"

# Add key:
# 	ssh-add /home/jonathan/Desktop/Embedded_Systems/Software/test-apps/x86_64/ssh-key-gen/stm32mp1
# 	ssh-copy-id -i /home/jonathan/Desktop/Embedded_Systems/Software/test-apps/x86_64/ssh-key-gen/stm32mp1.pub root@169.254.128.28
monitor_eth0: 
	ssh -t root@$(SSH_IPv4) 'cd /usr/bin/; exec /bin/sh --login'; exec bash; 

