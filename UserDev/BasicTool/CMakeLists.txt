# Nameing
get_filename_component(MODULE_NAME ${CMAKE_CURRENT_LIST_DIR} NAME)

# Include current path
include_directories(${CMAKE_CURRENT_LIST_DIR})

# module external libraries to link against
set(LIBS "")

# PYTHON
find_package(PythonLibs REQUIRED)
include_directories(${PYTHON_INCLUDE_DIRS})

# Directories
add_subdirectory(GeoAlgo)
add_subdirectory(FhiclLite)
add_subdirectory(EMShowerTools)
add_subdirectory(TreeGraph)
add_subdirectory(LArPyBackend)