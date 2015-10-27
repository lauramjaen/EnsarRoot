# Create a library called "libPassive" which includes the source files given in
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
${ENSARROOT_SOURCE_DIR}/calo/base 
${ENSARROOT_SOURCE_DIR}/calo/passive  
${ENSARROOT_SOURCE_DIR}/ensarbase  
#${CMAKE_SOURCE_DIR}/field 
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
R3BGeoPassivePar.cxx    
R3BGeoPipe.cxx        
R3BGeoTarget.cxx      
R3BPipe.cxx           
R3BTarget.cxx 
)

# fill list of header files from list of source files
# by exchanging the file extension
CHANGE_FILE_EXTENSION(*.cxx *.h HEADERS "${SRCS}")

Set(LINKDEF R3BCaloPassiveLinkDef.h)
Set(LIBRARY_NAME R3BCaloPassive) 

GENERATE_LIBRARY()
