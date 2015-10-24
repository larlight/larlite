cmake_minimum_required(VERSION 3.1)
project(core)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -g -O3 -std=c++0x -W -Wall -Wno-deprecated -fPIC -D_CORE_LINUX -pthread -m64")
set(CMAKE_CXX_COMPILER "/usr/bin/clang++")
#set({CMAKE_LINKER} "/usr/bin/g++")
set(CMAKE_SHARED_LINKER_FLAGS "-rdynamic")
cmake_minimum_required(VERSION 3.1)

if(DEFINED ENV{LARLITE_BASEDIR})
	message("Found LArLite in " $ENV{LARLITE_BASEDIR})
else()
	message(FATAL_ERROR "LArLite was not configured properly, execute config/setup.sh")
endif()

# Nameing
get_filename_component(MODULE_NAME ${CMAKE_CURRENT_LIST_DIR} NAME)

set(CMAKE_INSTALL_PREFIX $ENV{LARLITE_BASEDIR})
set(CMAKE_INSTALL_LIBDIR  $ENV{LARLITE_BASEDIR}/lib)

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
#include_directories(${ROOT_INCLUDE_DIR})

include_directories(${CMAKE_SOURCE_DIR} ${ROOT_INCLUDE_DIRS})
add_definitions(${ROOT_CXX_FLAGS})

# PYTHON
find_package(PythonLibs REQUIRED)
include_directories(${PYTHON_INCLUDE_DIRS})

include_directories($ENV{LARLITE_COREDIR})
# Building

add_subdirectory(GeoAlgo)
add_subdirectory(FhiclLite)
add_subdirectory(EMShowerTools)
add_subdirectory(TreeGraph)
add_subdirectory(LArPyBackend)




