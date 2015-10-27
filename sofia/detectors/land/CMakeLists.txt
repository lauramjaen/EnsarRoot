# Create a library called "libSOFIALand" which includes the source files given in
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
${ENSARROOT_SOURCE_DIR}/sofia/data/
${ENSARROOT_SOURCE_DIR}/sofia/data/calData
${ENSARROOT_SOURCE_DIR}/sofia/data/landData
${ENSARROOT_SOURCE_DIR}/sofia/detectors/twinmusic
${ENSARROOT_SOURCE_DIR}/sofia/mcstack 
${ENSARROOT_SOURCE_DIR}/sofia/passive
#put here all directories where header files are located
${ENSARROOT_SOURCE_DIR}/sofia/detectors/land
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
R3BConstantFraction.cxx
R3BGeoLand.cxx    
R3BGeoLandPar.cxx 
R3BLand.cxx  
R3BLandContFact.cxx 
R3BLandData.cxx 
R3BLandDigitizer.cxx 
R3BLandDigitizer_CFD.cxx
R3BLandDigiAnalyser.cxx
R3BLandDigiPar.cxx
R3BNeuLand.cxx   
R3BNeuLandCluster.cxx
R3BNeuLandClusterFinder.cxx
R3BNeuLandv1.cxx   
R3BNeuLandDigitizer.cxx  
R3BNeutHit.cxx
R3BNeutronCalibr2D.cxx
R3BNeutronTracker.cxx
R3BNeutronTracker2D.cxx
R3BPrimPart.cxx
)

# fill list of header files from list of source files
# by exchanging the file extension
CHANGE_FILE_EXTENSION(*.cxx *.h HEADERS "${SRCS}")

set(LINKDEF LandLinkDef.h)
set(LIBRARY_NAME SOFIALand)
set(DEPENDENCIES
    Base FairTools GeoBase ParBase SOFIAbase SOFIAData)

GENERATE_LIBRARY()
