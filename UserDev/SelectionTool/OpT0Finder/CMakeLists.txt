# Nameing
set(MODULE_NAME "SelectionTool_OpT0Finder")

# Include BasicTool
include_directories($ENV{LARLITE_USERDEVDIR}/BasicTool)

add_subdirectory(Base)
add_subdirectory(PhotonLibrary)
add_subdirectory(Algorithms)
add_subdirectory(App)
add_subdirectory(Ana)