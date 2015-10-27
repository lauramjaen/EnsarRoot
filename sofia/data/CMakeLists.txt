# Create a library called "libSOFIADATA" which includes the source files given in
# the array .
# Author: J.L. Rodriguez

Set(SYSTEM_INCLUDE_DIRECTORIES 
${SYSTEM_INCLUDE_DIRECTORIES}
${BASE_INCLUDE_DIRECTORIES}
)

set(INCLUDE_DIRECTORIES
${ROOT_INCLUDE_DIR} 
${BASE_INCLUDE_DIRECTORIES}
#put here all directories where header files are located
${ENSARROOT_SOURCE_DIR}/sofia/data  
${ENSARROOT_SOURCE_DIR}/sofia/data/mwpc1Data
${ENSARROOT_SOURCE_DIR}/sofia/detectors/land
${ENSARROOT_SOURCE_DIR}/sofia/detectors/mwpc_1
${ENSARROOT_SOURCE_DIR}/sofia/detectors/mwpc_2
${ENSARROOT_SOURCE_DIR}/sofia/detectors/stof
${ENSARROOT_SOURCE_DIR}/sofia/detectors/twinmusic

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
#caloData/R3BCaloCrystalHit.cxx
#caloData/R3BCaloCrystalHitSim.cxx
#caloData/R3BCaloHit.cxx
#caloData/R3BCaloHitSim.cxx
#caloData/R3BCaloPoint.cxx 
landData/R3BLandPoint.cxx
landData/R3BNeuLandPoint.cxx 
landData/R3BLandDigi.cxx 
landData/R3BLandFirstHits.cxx 
landData/R3BLandHit.cxx 
landData/R3BPaddleCrystalHit.cxx
landData/R3BNeutronTrack.cxx
musicData/R3BMusicRaw.cxx
mwpc1Data/R3BMWpcHit.cxx
mwpc1Data/R3BMWPCPads.cxx
mwpc1Data/R3BMWpcPoint.cxx
mwpc1Data/R3BMWPC1Raw.cxx
mwpc2Data/R3BMWpc2Hit.cxx
mwpc2Data/R3BMWpc2Point.cxx
mwpc2Data/R3BMWPC2Raw.cxx
startFFData/R3BStartFFRaw.cxx
stofData/R3BTofPoint.cxx
stofData/R3BToFWRaw.cxx
tpcData/R3BTPCRaw.cxx
twinmusicData/R3BTwinMusicPoint.cxx
twinmusicData/R3BTwinMusicRaw.cxx
)


# fill list of header files from list of source files
# by exchanging the file extension
CHANGE_FILE_EXTENSION(*.cxx *.h HEADERS "${SRCS}")
SET(HEADERS ${HEADERS} R3BDetectorList.h )

set(LINKDEF  R3BDataLinkDef.h)
set(LIBRARY_NAME SOFIAData)
set(DEPENDENCIES
    Base FairTools)

GENERATE_LIBRARY()
