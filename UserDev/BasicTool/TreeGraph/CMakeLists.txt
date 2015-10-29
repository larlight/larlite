
# Generare the library names based on the directory
get_filename_component(DIR ${CMAKE_CURRENT_LIST_DIR} NAME)
set(LIB_NAME ${MODULE_NAME}_${DIR})
set(DICT_NAME "${LIB_NAME}Dict")

# Get all the header files
FILE(GLOB HEADERS "*.h")
LIST(REMOVE_ITEM HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/LinkDef.h)
set(HEADER_FILES ${HEADERS})

# Get all the cxx files in a list
FILE(GLOB EXECUTABLES "*.cxx")
set(EXEC ${EXECUTABLES})

# generate rootmap
ROOT_GENERATE_DICTIONARY(${DICT_NAME} ${HEADER_FILES} LINKDEF LinkDef.h OPTIONS -p)

add_library(${LIB_NAME} SHARED ${EXEC} ${DICT_NAME}.cxx)

target_link_libraries(${LIB_NAME} ${ROOT_LIBRARIES} ${LIBS})

# installing library, rootmap and pcm to LArLite library path
install(TARGETS ${LIB_NAME} DESTINATION lib)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/lib${DICT_NAME}.rootmap DESTINATION lib RENAME lib${LIB_NAME}.rootmap)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${DICT_NAME}_rdict.pcm DESTINATION lib)# RENAME lib${LIB_NAME}_rdict.pcm)