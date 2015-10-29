# Nameing
get_filename_component(MODULE_NAME ${CMAKE_CURRENT_LIST_DIR} NAME)

# Include current path
include_directories(${CMAKE_CURRENT_LIST_DIR})

# Include larlite core & userdev directroies
include_directories($ENV{LARLITE_COREDIR})

# module external libraries to link against
set(LIBS "")

add_subdirectory(CCQESelection)
add_subdirectory(ERTool)
add_subdirectory(FluxRW_test)
add_subdirectory(OpT0Finder)
#add_subdirectory(ShowerStudy)