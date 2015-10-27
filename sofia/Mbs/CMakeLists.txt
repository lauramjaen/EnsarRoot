# Create a library called "libR3BMbs" which includes the source files given in
# the array .
# Author: J.L. Rodriguez

Set(SYSTEM_INCLUDE_DIRECTORIES 
${SYSTEM_INCLUDE_DIRECTORIES}
${BASE_INCLUDE_DIRECTORIES}
)

set(INCLUDE_DIRECTORIES
${ROOT_INCLUDE_DIR} 
${BASE_INCLUDE_DIRECTORIES}
#put here all directories where header files are located
${ENSARROOT_SOURCE_DIR}/sofia/Mbs 
)

include_directories( ${INCLUDE_DIRECTORIES})
include_directories(SYSTEM ${SYSTEM_INCLUDE_DIRECTORIES})

set(LINK_DIRECTORIES
${ROOT_LIBRARY_DIR}
${FAIRROOT_LIBRARY_DIR}
)
 
link_directories( ${LINK_DIRECTORIES})

 
# Really lazy version to get the list of source files
# Take all source file in the given source dir
#FILE(GLOB MBS_SRCS ${CMAKE_CURRENT_SOURCE_DIR} *.cxx)

# List of source files
set(SRCS
  R3BMbsFile.cxx
)

# fill list of header files from list of source files
# by exchanging the file extension
CHANGE_FILE_EXTENSION(*.cxx *.h HEADERS "${SRCS}")

set(LINKDEF MbsLinkDef.h)
Set(LIBRARY_NAME R3BMbs)
Set(DEPENDENCIES
    MbsAPI)

GENERATE_LIBRARY()
