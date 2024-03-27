# ============================================================================================
# 
#  Author:       Jonathan Edwards
#  Date:         15/11/2022
# 
#  Description:  
# 				Compiles all source C and C++ files into object files that will be used
# 					for linking. 
# 
#  ============================================================================================


# App Name
TARGET_1  = CaptureImage
TARGET_1l = capture_image


# Generating Object Names
SOURCES_LOCATION  = $(TARGET_1)/src

SOURCES += $(wildcard $(SOURCES_LOCATION)/*.c)
OBJECTS += $(patsubst $(SOURCES_LOCATION)/%.c, %.o, $(SOURCES))

SOURCES += $(wildcard $(SOURCES_LOCATION)/*.cpp)
OBJECTS += $(patsubst $(SOURCES_LOCATION)/%.cpp, %.o, $(SOURCES))


# Compile: create object files from C source files
%.o: $(SOURCES_LOCATION)/%.c
	$(SETUP) \
	$(CC) -c $(CFLAGS) -o $(TARGET_1)/build/$@ $< $(INCLUDES_1)
	@echo

# Compile: create object files from C++ source files
%.o: $(SOURCES_LOCATION)/%.cpp
	$(SETUP) \
	$(CXX) -c $(CXXFLAGS) -o $(TARGET_1)/build/$@ $< $(INCLUDES_1) $(LIBS_1)
	@echo
	
