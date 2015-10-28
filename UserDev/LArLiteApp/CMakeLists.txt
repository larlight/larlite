
# Nameing
get_filename_component(MODULE_NAME ${CMAKE_CURRENT_LIST_DIR} NAME)

include_directories(${CMAKE_CURRENT_LIST_DIR})
include_directories($ENV{LARLITE_COREDIR})
include_directories($ENV{LARLITE_USERDEVDIR})
include_directories($ENV{LARLITE_USERDEVDIR}/SelectionTool)
include_directories($ENV{LARLITE_USERDEVDIR}/BasicTool)

include_directories($ENV{LARLITE_USERDEVDIR}/LArLiteApp/SingleGamma)

#ERToolBackend CCSingleE Pi0Ana SingleGamma GraphMaker EMPart MichelE fluxRW
add_subdirectory(CCSingleE)
add_subdirectory(ERToolBackend)
add_subdirectory(Pi0Ana)
add_subdirectory(SingleGamma)
add_subdirectory(GraphMaker)
add_subdirectory(EMPart)
add_subdirectory(MichelE)
add_subdirectory(fluxRW)
#add_subdirectory(ShowerStudy)


