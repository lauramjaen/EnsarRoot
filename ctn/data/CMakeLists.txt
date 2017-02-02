# Create a library called "libEnsarHPGeDetDATA" which includes the source files given in
# the array .
# The extension is already found.  Any number of sources could be listed here.

Set(SYSTEM_INCLUDE_DIRECTORIES 
${SYSTEM_INCLUDE_DIRECTORIES}
${BASE_INCLUDE_DIRECTORIES}
)

set(INCLUDE_DIRECTORIES
${ROOT_INCLUDE_DIR} 
${BASE_INCLUDE_DIRECTORIES}
#put here all directories where header files are located
${ENSARROOT_SOURCE_DIR}/ctn/data  
)

include_directories( ${INCLUDE_DIRECTORIES})
include_directories(SYSTEM ${SYSTEM_INCLUDE_DIRECTORIES})

set(LINK_DIRECTORIES
${ROOT_LIBRARY_DIR}
${FAIRROOT_LIBRARY_DIR}
)
 
link_directories( ${LINK_DIRECTORIES})

set(SRCS
EnsarHPGeDetHit.cxx
EnsarHPGeDetPoint.cxx 
)


# fill list of header files from list of source files
# by exchanging the file extension
CHANGE_FILE_EXTENSION(*.cxx *.h HEADERS "${SRCS}")
SET(HEADERS ${HEADERS} )
#SET(HEADERS ${HEADERS} EnsarHPGeDetList.h )

set(LINKDEF  EnsarHPGeDetDataLinkDef.h)
set(LIBRARY_NAME EnsarHPGeDetData)
set(DEPENDENCIES
    Base FairTools)

GENERATE_LIBRARY()

