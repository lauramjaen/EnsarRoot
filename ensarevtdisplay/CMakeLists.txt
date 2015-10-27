# Create a library called "libEnsarEvtVis" 

Set(SYSTEM_INCLUDE_DIRECTORIES 
${SYSTEM_INCLUDE_DIRECTORIES}
${BASE_INCLUDE_DIRECTORIES}
)

set(INCLUDE_DIRECTORIES
${ROOT_INCLUDE_DIR} 
${BASE_INCLUDE_DIRECTORIES}
${GEANT3_INCLUDE_DIR}
#put here all directories where header files are located
${ENSARROOT_SOURCE_DIR}/base 
${ENSARROOT_SOURCE_DIR}/ensardata
${ENSARROOT_SOURCE_DIR}/ensarevtdisplay
${ENSARROOT_SOURCE_DIR}/eventdisplay
${ENSARROOT_SOURCE_DIR}/geane
${ENSARROOT_SOURCE_DIR}/mcstack
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
EnsarEventManager.cxx
EnsarEventManagerEditor.cxx
EnsarIonName.cxx
EnsarMCTracks.cxx
EnsarMultiView.cxx
)

# fill list of header files from list of source files
# by exchanging the file extension
CHANGE_FILE_EXTENSION(*.cxx *.h HEADERS "${SRCS}")

Set(LINKDEF EnsarEventDisplayLinkDef.h)
Set(LIBRARY_NAME EnsarEvtVis)
Set(DEPENDENCIES
     Base EventDisplay FairTools Geane GeoBase ParBase)

GENERATE_LIBRARY()
