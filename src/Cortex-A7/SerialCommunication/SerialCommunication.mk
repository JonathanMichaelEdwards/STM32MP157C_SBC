# ============================================================================================
# 
#  Author:       Jonathan Edwards
#  Date:         15/11/2022
# 
#  Description:  
# 				Compiles all source C files into object files that will be used
# 					for linking. 
# 
#  ============================================================================================



# App Name
TARGET_2  = SerialCommunication
TARGET_2l = serial_communication


# Generating Object Names
SOURCES_LOCATION_COM  = $(TARGET_2)/src

SOURCES_COM += $(wildcard $(SOURCES_LOCATION_COM)/*.c)
OBJECTS_COM += $(patsubst $(SOURCES_LOCATION_COM)/%.c, %.o, $(SOURCES_COM))


# Compile: create object files from C source files
%.o: $(SOURCES_LOCATION_COM)/%.c
	gcc -c -m64 $(OPT) -Wall -std=gnu11 -o $(TARGET_2)/build/$@ $< -I./$(TARGET_2)/inc/
	@echo

