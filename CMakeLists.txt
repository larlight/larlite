cmake_minimum_required(VERSION 3.1)
project(larlite)

# Just copied the flags from what was used by makefiles
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -g -O3 -std=c++0x -W -Wall -Wno-deprecated -fPIC -D_CORE_LINUX -pthread -m64")
set(CMAKE_CXX_COMPILER "/usr/bin/clang++")
#set({CMAKE_LINKER} "/usr/bin/g++")
set(CMAKE_SHARED_LINKER_FLAGS "-rdynamic")

# no clue if thats true...
cmake_minimum_required(VERSION 3.1)

# Check if LArlite was configured
if(DEFINED ENV{LARLITE_BASEDIR})
	message("Found LArLite in " $ENV{LARLITE_BASEDIR})
	set(BASEDIR $ENV{LARLITE_BASEDIR})
else()
	message(FATAL_ERROR "LArLite was not configured properly, execute config/setup.sh")
endif()

# ======================================================================================================
# ========================================== ROOT STUFF ================================================
# You need to tell CMake where to find the ROOT installation. This can be done in a number of ways:
#   - ROOT built with classic configure/make use the provided $ROOTSYS/etc/cmake/FindROOT.cmake
#   - ROOT built with CMake. Add in CMAKE_PREFIX_PATH the installation prefix for ROOT

#set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} $ENV{ROOTSYS}/etc/cmake/)
list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS})

#---Locate the ROOT package and defines a number of variables (e.g. ROOT_INCLUDE_DIRS)
find_package(ROOT REQUIRED COMPONENTS Core RIO Net Hist Graf Graf3d Gpad Tree Rint Postscript Matrix Physics MathCore Thread)

#---Define useful ROOT functions and macros (e.g. ROOT_GENERATE_DICTIONARY)
include(${ROOT_USE_FILE})


# Define where to put the compiled libraries
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${BASEDIR}/lib)

# Definitions for installing
set(CMAKE_INSTALL_PREFIX ${BASEDIR})
set(CMAKE_INSTALL_LIBDIR  ${BASEDIR}/lib)

# Global include directories
include_directories(${CMAKE_SOURCE_DIR} ${ROOT_INCLUDE_DIRS})
add_definitions(${ROOT_CXX_FLAGS})

# Building
add_subdirectory(core)
add_subdirectory(UserDev)



