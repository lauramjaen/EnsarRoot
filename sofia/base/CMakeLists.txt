# Create a library called "libSOFIAbase" which includes the source files given in
# the array .
# The extension is already found.  Any number of sources could be listed here.

Set(SYSTEM_INCLUDE_DIRECTORIES 
${SYSTEM_INCLUDE_DIRECTORIES}
${BASE_INCLUDE_DIRECTORIES}
)

set(INCLUDE_DIRECTORIES
${ROOT_INCLUDE_DIR} 
${BASE_INCLUDE_DIRECTORIES}
${ENSARROOT_SOURCE_DIR}/fairtools
${ENSARROOT_SOURCE_DIR}/sofia/Mbs
#put here all directories where header files are located
${ENSARROOT_SOURCE_DIR}/sofia/base 
)

include_directories( ${INCLUDE_DIRECTORIES})
include_directories(SYSTEM ${SYSTEM_INCLUDE_DIRECTORIES})

set(LINK_DIRECTORIES
${ROOT_LIBRARY_DIR}
${FAIRROOT_LIBRARY_DIR}
) 

link_directories( ${LINK_DIRECTORIES})

set(SRCS
#Put here your sourcefiles
R3BDetector.cxx 
R3BModule.cxx 
)

# fill list of header files from list of source files
# by exchanging the file extension
CHANGE_FILE_EXTENSION(*.cxx *.h HEADERS "${SRCS}")

set(LINKDEF R3BLinkDef.h)
Set(LIBRARY_NAME SOFIAbase)
Set(DEPENDENCIES
    Core FairTools GenVector GeoBase Geom MathCore Matrix ParBase  Physics)

GENERATE_LIBRARY()
