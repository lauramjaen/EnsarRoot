# Create a library called "libFairTestDetector" which includes the source files given in
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
${ENSARROOT_SOURCE_DIR}/mcstack
${ENSARROOT_SOURCE_DIR}/E105
${ENSARROOT_SOURCE_DIR}/E105/data
${ENSARROOT_SOURCE_DIR}/E105/dssd
${ENSARROOT_SOURCE_DIR}/E105/sili
${ENSARROOT_SOURCE_DIR}/E105/passive
${ENSARROOT_SOURCE_DIR}/E105/E105Gen
)

include_directories( ${INCLUDE_DIRECTORIES})
include_directories(SYSTEM ${SYSTEM_INCLUDE_DIRECTORIES})

set(LINK_DIRECTORIES
${ROOT_LIBRARY_DIR}
${FAIRROOT_LIBRARY_DIR}
) 

link_directories( ${LINK_DIRECTORIES})

Set(SRCS

dssd/E105DSSDContFact.cxx
dssd/E105DSSDConstPar.cxx
dssd/E105DSSDGeoPar.cxx
dssd/E105DSSDMapPar.cxx
dssd/E105DSSDGeo.cxx
dssd/E105DSSD.cxx

sili/E105SiLiContFact.cxx
sili/E105SiLiConstPar.cxx
sili/E105SiLiGeoPar.cxx
sili/E105SiLiMapPar.cxx
sili/E105SiLiGeo.cxx
sili/E105SiLi.cxx

#passive/E105GeoPassivePar.cxx
#passive/E105GeoSlit.cxx
#passive/E105Slit.cxx
#passive/E105GeoShield.cxx
#passive/E105Shield.cxx
########
#passive/E105ContFact.cxx
#passive/E105SlitPar.cxx
#passive/E105AccessRTDBTask.cxx

########
data/E105DSSDPoint.cxx
data/E105DSSDHit.cxx
data/E105SiLiPoint.cxx
data/E105SiLiHit.cxx
E105Gen/E105AsciiGenerator.cxx
)

# fill list of header files from list of source files
# by exchanging the file extension
CHANGE_FILE_EXTENSION(*.cxx *.h HEADERS "${SRCS}")
SET(HEADERS ${HEADERS} ${FAIRROOTPATH}/include/FairDbReader.h ${FAIRROOTPATH}/include/FairDbWriter.h)

set(LINKDEF EnsarE105LinkDef.h)
set(LIBRARY_NAME E105)
set(DEPENDENCIES
    Base)

GENERATE_LIBRARY()
