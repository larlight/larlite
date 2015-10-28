
# Nameing
get_filename_component(MODULE_NAME ${CMAKE_CURRENT_LIST_DIR} NAME)

if(MODULE_NAME MATCHES "core")
	# We are building larlite core
	set(MODULE_NAME LArLite)
endif()

include_directories(${CMAKE_CURRENT_LIST_DIR})

# PYTHON
find_package(PythonLibs REQUIRED)
include_directories(${PYTHON_INCLUDE_DIRS})


add_subdirectory(GeoAlgo)
add_subdirectory(FhiclLite)
add_subdirectory(EMShowerTools)
add_subdirectory(TreeGraph)
add_subdirectory(LArPyBackend)




