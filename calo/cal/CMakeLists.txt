# Create a library called "R3BCaloCal" which includes the source files given in
# the array .
# The extension is already found.  Any number of sources could be listed here.
add_subdirectory (unpacker)

Set(SYSTEM_INCLUDE_DIRECTORIES 
${SYSTEM_INCLUDE_DIRECTORIES}
${BASE_INCLUDE_DIRECTORIES}
)


set(INCLUDE_DIRECTORIES
${ROOT_INCLUDE_DIR} 
#put here all directories where header files are located
${ENSARROOT_SOURCE_DIR}/calo/cal
${ENSARROOT_SOURCE_DIR}/calo/cal/unpack
${ENSARROOT_SOURCE_DIR}/calo/cal/unpacker
${ENSARROOT_SOURCE_DIR}/calo/data
${ENSARROOT_SOURCE_DIR}/calo/db
${ENSARROOT_SOURCE_DIR}/calo/commondb
${ENSARROOT_SOURCE_DIR}/calo/passive 
${ENSARROOT_SOURCE_DIR}/ensarbase
${ENSARROOT_SOURCE_DIR}/ensardata
${ENSARROOT_SOURCE_DIR}/input
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
R3BCalo.cxx
R3BCaloContFact.cxx
R3BCaloHitFinder.cxx  
R3BCaloHitFinderPar.cxx  
R3BGeoCalo.cxx   
R3BGeoCaloPar.cxx   
unpack/R3BCaloRawHit.cxx
unpack/R3BCaloUnpack.cxx
unpack/R3BCaloUnpackPar.cxx
unpack/R3BCaloRawAna.cxx
#unpack/R3BCaloCal.cxx
#unpack/R3BCaloCrystalAna.cxx
)

# fill list of header files from list of source files
# by exchanging the file extension
CHANGE_FILE_EXTENSION(*.cxx *.h HEADERS "${SRCS}")
#SET(HEADERS ${HEADERS} ${FAIRROOTPATH}/include/FairDbReader.h ${FAIRROOTPATH}/include/FairDbWriter.h)

set(LINKDEF R3BCaloCalLinkDef.h)
set(LIBRARY_NAME R3BCaloCal)
set(DEPENDENCIES
    EnsarBase EnsarData R3BCaloData R3BCaloPassive)

if(FAIRDB_FOUND)
set(DEPENDENCIES
    ${DEPENDENCIES} FairDB)
endif(FAIRDB_FOUND)

GENERATE_LIBRARY()

