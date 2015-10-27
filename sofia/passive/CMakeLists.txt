# Create a library called "libSOFIAPassive" which includes the source files given in
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
${ENSARROOT_SOURCE_DIR}/sofia/base  
${ENSARROOT_SOURCE_DIR}/sofia/field 
${ENSARROOT_SOURCE_DIR}/sofia/passive  
)

include_directories( ${INCLUDE_DIRECTORIES})
include_directories(SYSTEM ${SYSTEM_INCLUDE_DIRECTORIES})

set(LINK_DIRECTORIES
${ROOT_LIBRARY_DIR}
${FAIRROOT_LIBRARY_DIR}
)
 
link_directories( ${LINK_DIRECTORIES})

set(SRCS
R3BCave.cxx            
R3BGeoCave.cxx        
R3BGeoMagnet.cxx            
R3BGeoPassivePar.cxx        
R3BGladMagnet.cxx
R3BMagnet.cxx    
R3BPassiveContFact.cxx 
R3BTarget.cxx    
R3Btubehelium.cxx 
R3Btubevacuum.cxx 
)

# fill list of header files from list of source files
# by exchanging the file extension
CHANGE_FILE_EXTENSION(*.cxx *.h HEADERS "${SRCS}")

Set(LINKDEF PassiveLinkDef.h)
Set(LIBRARY_NAME SOFIAPassive) 
Set(DEPENDENCIES
    Field SOFIAbase)

GENERATE_LIBRARY()
