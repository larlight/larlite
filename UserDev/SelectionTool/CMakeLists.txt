
# Nameing
get_filename_component(MODULE_NAME ${CMAKE_CURRENT_LIST_DIR} NAME)

include_directories(${CMAKE_CURRENT_LIST_DIR})
include_directories($ENV{LARLITE_COREDIR})
include_directories($ENV{LARLITE_USERDEVDIR})

add_subdirectory(CCQESelection)
add_subdirectory(ERTool)
add_subdirectory(FluxRW_test)
add_subdirectory(OpT0Finder)
#add_subdirectory(ShowerStudy)


