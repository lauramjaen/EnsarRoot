# Create a library called "libEnsarDATA" which includes the source files given in
# the array .
# The extension is already found.  Any number of sources could be listed here.

Set(SYSTEM_INCLUDE_DIRECTORIES 
${SYSTEM_INCLUDE_DIRECTORIES}
${BASE_INCLUDE_DIRECTORIES}
)

set(INCLUDE_DIRECTORIES
${ROOT_INCLUDE_DIR}
${PLUTO_INCLUDE_DIR} 
${BASE_INCLUDE_DIRECTORIES}
${ENSARROOT_SOURCE_DIR}/calo/data/caloData
${ENSARROOT_SOURCE_DIR}/ensardata  
)

include_directories( ${INCLUDE_DIRECTORIES})
include_directories(SYSTEM ${SYSTEM_INCLUDE_DIRECTORIES})

set(LINK_DIRECTORIES
${ROOT_LIBRARY_DIR}
${FAIRROOT_LIBRARY_DIR}
)
 
link_directories( ${LINK_DIRECTORIES})

set(SRCS
EnsarMCStack.cxx
EnsarMCTrack.cxx
mydetData/EnsarMyDetPoint.cxx
)

# fill list of header files from list of source files
# by exchanging the file extension
CHANGE_FILE_EXTENSION(*.cxx *.h HEADERS "${SRCS}")
SET(HEADERS ${HEADERS} EnsarDetectorList.h )

set(LINKDEF  EnsarDataLinkDef.h)
set(LIBRARY_NAME EnsarData)
set(DEPENDENCIES
    Base FairTools)

GENERATE_LIBRARY()
