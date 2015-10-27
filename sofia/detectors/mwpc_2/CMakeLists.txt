# Create a library called "libSOFIAMwpc2" which includes the source files given in
# the array .
# Author: J.L. Rodriguez

Set(SYSTEM_INCLUDE_DIRECTORIES 
${SYSTEM_INCLUDE_DIRECTORIES}
${BASE_INCLUDE_DIRECTORIES}
)

set(INCLUDE_DIRECTORIES
${ROOT_INCLUDE_DIR} 
${BASE_INCLUDE_DIRECTORIES}
${ENSARROOT_SOURCE_DIR}/sofia/base 
${ENSARROOT_SOURCE_DIR}/sofia/data/calData
${ENSARROOT_SOURCE_DIR}/sofia/data/caloData
${ENSARROOT_SOURCE_DIR}/sofia/data/landData
${ENSARROOT_SOURCE_DIR}/sofia/data/mwpc2Data
${ENSARROOT_SOURCE_DIR}/sofia/data/stofData
${ENSARROOT_SOURCE_DIR}/sofia/data/twinmusicData
${ENSARROOT_SOURCE_DIR}/sofia/detectors/cal 
${ENSARROOT_SOURCE_DIR}/sofia/mcstack 
${ENSARROOT_SOURCE_DIR}/sofia/passive 
#put here all directories where header files are located
${ENSARROOT_SOURCE_DIR}/sofia/detectors/mwpc_2
)

include_directories( ${INCLUDE_DIRECTORIES})
include_directories(SYSTEM ${SYSTEM_INCLUDE_DIRECTORIES})

set(LINK_DIRECTORIES
${ROOT_LIBRARY_DIR}
${FAIRROOT_LIBRARY_DIR}
)
 
link_directories( ${LINK_DIRECTORIES})

set(SRCS
R3BGeoMWpc.cxx
R3BGeoMWpcPar.cxx
R3BMWpc2.cxx
)

# fill list of header files from list of source files
# by exchanging the file extension
CHANGE_FILE_EXTENSION(*.cxx *.h HEADERS "${SRCS}")

set(LINKDEF mwpc2LinkDef.h)
set(LIBRARY_NAME SOFIAMWpc2)
set(DEPENDENCIES
    Base FairTools GeoBase ParBase SOFIAbase SOFIAData)

GENERATE_LIBRARY()
