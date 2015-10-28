
# Nameing
get_filename_component(MODULE_NAME ${CMAKE_CURRENT_LIST_DIR} NAME)

if(MODULE_NAME MATCHES "core")
	# We are building larlite core
	set(MODULE_NAME LArLite)
endif()

include_directories(${CMAKE_CURRENT_LIST_DIR})

add_subdirectory(Base)
add_subdirectory(LArUtil)
add_subdirectory(DataFormat)
add_subdirectory(Analysis)



