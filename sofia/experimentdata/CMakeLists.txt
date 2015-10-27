# Create a library called "libR3BTRootEvent" which includes the source files given in
# the array .
# Author: J.L. Rodriguez

Set(SYSTEM_INCLUDE_DIRECTORIES 
${SYSTEM_INCLUDE_DIRECTORIES}
${BASE_INCLUDE_DIRECTORIES}
)

set(INCLUDE_DIRECTORIES
${ROOT_INCLUDE_DIR} 
#put here all directories where header files are located
${ENSARROOT_SOURCE_DIR}/base
${ENSARROOT_SOURCE_DIR}/sofia/experimentdata
)

include_directories( ${INCLUDE_DIRECTORIES})
include_directories(SYSTEM ${SYSTEM_INCLUDE_DIRECTORIES})

set(LINK_DIRECTORIES
${ROOT_LIBRARY_DIR}
)
 
link_directories( ${LINK_DIRECTORIES})

# Really lazy version to get the list of source files
# Take all source file in the given source dir
#FILE(GLOB TRootEvent_SRCS ${CMAKE_CURRENT_SOURCE_DIR} *.cxx)

# List of source files
set(TRootEvent_SRCS
)
# list of header files
set(TRootEvent_HEADERS
  TRootEvent.h
)

if(RULE_CHECKER_FOUND)
  CHECK_RULES("${TRootEvent_SRCS}" "${INCLUDE_DIRECTORIES}" TRootEvent_RULES)
endif(RULE_CHECKER_FOUND)

# set everything needed for the root dictonary and create the
# dictionary
set(TRootEvent_LINKDEF TRootEventLinkDef.h )
set(TRootEvent_DICTIONARY ${CMAKE_CURRENT_BINARY_DIR}/TRootEventDict.cxx) 
ROOT_GENERATE_DICTIONARY("${TRootEvent_HEADERS}" "${TRootEvent_LINKDEF}" "${TRootEvent_DICTIONARY}" "${INCLUDE_DIRECTORIES}")

# add the dictionary to the list of source files
SET(TRootEvent_SRCS ${TRootEvent_SRCS} ${TRootEvent_DICTIONARY}) 


############### build the library #####################
add_library(TRootEvent SHARED ${TRootEvent_SRCS})
target_link_libraries(TRootEvent ${ROOT_LIBRARIES})
set_target_properties(TRootEvent PROPERTIES ${FAIRROOT_LIBRARY_PROPERTIES})

############### install the library ###################
install(TARGETS TRootEvent DESTINATION ${CMAKE_BINARY_DIR}/lib)
