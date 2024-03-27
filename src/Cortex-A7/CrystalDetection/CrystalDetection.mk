# ============================================================================================
# 
#  Author:       Jonathan Edwards
#  Date:         20/11/2022
# 
#  Description:  
# 				Compiles all source C++ files into object files that will be used
# 					for linking. 
# 
#  ============================================================================================


# App Name
TARGET_3  = CrystalDetection
TARGET_3l = crystal_detection


# Generating Object Names
SOURCES_LOCATION_DET  = $(TARGET_3)/src


SOURCES_DET += $(wildcard $(SOURCES_LOCATION_DET)/*.cpp)
OBJECTS_DET += $(patsubst $(SOURCES_LOCATION_DET)/%.cpp, %.o, $(SOURCES_DET))


# Compile: create object files from C++ source files
%.o: $(SOURCES_LOCATION_DET)/%.cpp
	$(SETUP) \
	$(CXX) -c $(CXXFLAGS) -o $(TARGET_3)/build/$@ $< $(INCLUDES_3) $(LIBS_3)
	@echo
	
