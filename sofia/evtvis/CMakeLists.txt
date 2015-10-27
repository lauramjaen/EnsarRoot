# Create a library called "libSOFIAEvtVis" 

Set(SYSTEM_INCLUDE_DIRECTORIES 
${SYSTEM_INCLUDE_DIRECTORIES}
${BASE_INCLUDE_DIRECTORIES}
)

set(INCLUDE_DIRECTORIES
${ROOT_INCLUDE_DIR} 
${GEANT3_INCLUDE_DIR}
${BASE_INCLUDE_DIRECTORIES}
#put here all directories where header files are located
${ENSARROOT_SOURCE_DIR}/geane
${ENSARROOT_SOURCE_DIR}/sofia/evtvis
${ENSARROOT_SOURCE_DIR}/sofia/field 
${ENSARROOT_SOURCE_DIR}/sofia/mcstack
${ENSARROOT_SOURCE_DIR}/trackbase 
)

include_directories( ${INCLUDE_DIRECTORIES})
include_directories(SYSTEM ${SYSTEM_INCLUDE_DIRECTORIES})

set(LINK_DIRECTORIES
${ROOT_LIBRARY_DIR}
${FAIRROOT_LIBRARY_DIR}
)
 
link_directories( ${LINK_DIRECTORIES})

set(SRCS
R3BEventManager.cxx
R3BEventManagerEditor.cxx
R3BIonName.cxx
R3BMCTracks.cxx
)

# fill list of header files from list of source files
# by exchanging the file extension
CHANGE_FILE_EXTENSION(*.cxx *.h HEADERS "${SRCS}")

set(LINKDEF R3BEventDisplayLinkDef.h)
set(LIBRARY_NAME SOFIAEvtVis)
set(DEPENDENCIES
    EventDisplay Field Geane MCStack SOFIAData)

GENERATE_LIBRARY()
