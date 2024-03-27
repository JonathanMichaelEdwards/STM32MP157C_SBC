# ============================================================================================
# 
#  Author:       Jonathan Edwards
#  Date:         25/11/2022
# 
#  Description:  
# 				Compiles all source C files into object files that will be used
# 					for linking. 
# 
#  ============================================================================================


# App Name
TARGET_4  = Autofocus
TARGET_4l = autofocus


# Generating Object Names
SOURCES_LOCATION_AUTO  = $(TARGET_4)/src


SOURCES_AUTO += $(wildcard $(SOURCES_LOCATION_AUTO)/*.c)
OBJECTS_AUTO += $(patsubst $(SOURCES_LOCATION_AUTO)/%.c, %.o, $(SOURCES_AUTO))


# Compile: create object files from C source files
%.o: $(SOURCES_LOCATION_AUTO)/%.c
	$(SETUP) \
	$(CC) -c $(CFLAGS) -o $(TARGET_4)/build/$@ $< $(INCLUDES_4)
	@echo
	
