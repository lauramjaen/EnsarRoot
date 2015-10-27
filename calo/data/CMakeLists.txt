# Create a library called "libR3BDATA" which includes the source files given in
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
${ENSARROOT_SOURCE_DIR}/calo/data  
${ENSARROOT_SOURCE_DIR}/calo/data/caloData  
)

include_directories( ${INCLUDE_DIRECTORIES})
include_directories(SYSTEM ${SYSTEM_INCLUDE_DIRECTORIES})

set(LINK_DIRECTORIES
${ROOT_LIBRARY_DIR}
${FAIRROOT_LIBRARY_DIR}
)
 
link_directories( ${LINK_DIRECTORIES})

set(SRCS
R3BMCStack.cxx
R3BMCTrack.cxx
caloData/R3BCaloCrystalHit.cxx
caloData/R3BCaloCrystalHitSim.cxx
caloData/R3BCaloHit.cxx
caloData/R3BCaloHitSim.cxx
caloData/R3BCaloPoint.cxx 
)


# fill list of header files from list of source files
# by exchanging the file extension
CHANGE_FILE_EXTENSION(*.cxx *.h HEADERS "${SRCS}")
SET(HEADERS ${HEADERS} R3BDetectorList.h )

set(LINKDEF  R3BDataLinkDef.h)
set(LIBRARY_NAME R3BCaloData)
set(DEPENDENCIES
    Base FairTools)

GENERATE_LIBRARY()

