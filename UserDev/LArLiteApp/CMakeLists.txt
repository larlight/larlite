
# Nameing
get_filename_component(MODULE_NAME ${CMAKE_CURRENT_LIST_DIR} NAME)

# Include larlite core & userdev directroies
include_directories(${CMAKE_CURRENT_LIST_DIR})
include_directories($ENV{LARLITE_COREDIR})

# We need basic- and selectiontool
include_directories($ENV{LARLITE_USERDEVDIR}/SelectionTool)
include_directories($ENV{LARLITE_USERDEVDIR}/BasicTool)

# Seems that single gamma needs that
include_directories($ENV{LARLITE_USERDEVDIR}/LArLiteApp/SingleGamma)

add_subdirectory(CCSingleE)
add_subdirectory(ERToolBackend)
add_subdirectory(Pi0Ana)
add_subdirectory(SingleGamma)
add_subdirectory(GraphMaker)
add_subdirectory(EMPart)
add_subdirectory(MichelE)
add_subdirectory(fluxRW)
#add_subdirectory(ShowerStudy)


