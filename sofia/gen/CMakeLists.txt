# Create a library called "libSOFIAGen" which includes the source files given in
# the array .
# Author: J.L. Rodriguez

Set(SYSTEM_INCLUDE_DIRECTORIES 
${SYSTEM_INCLUDE_DIRECTORIES}
${BASE_INCLUDE_DIRECTORIES}
)

set(INCLUDE_DIRECTORIES
${ROOT_INCLUDE_DIR}
${PLUTO_INCLUDE_DIR} 
${BASE_INCLUDE_DIRECTORIES}
#put here all directories where header files are located
${ENSARROOT_SOURCE_DIR}/sofia/field 
${ENSARROOT_SOURCE_DIR}/sofia/gen
${ENSARROOT_SOURCE_DIR}/sofia/generators 
)

include_directories( ${INCLUDE_DIRECTORIES})
include_directories(SYSTEM ${SYSTEM_INCLUDE_DIRECTORIES})

set(LINK_DIRECTORIES
${ROOT_LIBRARY_DIR}
${PLUTO_LIBRARY_DIR}
${FAIRROOT_LIBRARY_DIR}
)
 
link_directories( ${LINK_DIRECTORIES})

set(SRCS
R3BBackTracking.cxx
R3BBackTrackingStorageState.cxx
#R3BCALIFATestGenerator.cxx
R3BCosmicGenerator.cxx
R3BIonGenerator.cxx
R3BLandGenerator.cxx
R3Bp2pGenerator.cxx
R3BReadKinematics.cxx
)

# fill list of header files from list of source files
# by exchanging the file extension
CHANGE_FILE_EXTENSION(*.cxx *.h HEADERS "${SRCS}")

set(LINKDEF R3BGenLinkDef.h)
set(LIBRARY_NAME SOFIAGen)
set(DEPENDENCIES
    Field Gen)

GENERATE_LIBRARY()

