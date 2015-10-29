
# Nameing
set(MODULE_NAME LArLite)

# Include current path
include_directories(${CMAKE_CURRENT_LIST_DIR})

# module external libraries to link against
set(LIBS "")

# Modules to build
add_subdirectory(Base)
add_subdirectory(LArUtil)
add_subdirectory(DataFormat)
add_subdirectory(Analysis)