# Create a library called "libSOFIAPlist" which includes the source files
# given in # the array . # The extension is already found.  Any number of
# sources could be listed here.

Set(SYSTEM_INCLUDE_DIRECTORIES 
${SYSTEM_INCLUDE_DIRECTORIES}
${BASE_INCLUDE_DIRECTORIES}
)

set(INCLUDE_DIRECTORIES
${ROOT_INCLUDE_DIR} 
${GEANT4_INCLUDE_DIR}
${CLHEP_INCLUDE_DIR}
${GEANT4VMC_INCLUDE_DIR}
${BASE_INCLUDE_DIRECTORIES}
#put here all directories where header files are located
${ENSARROOT_SOURCE_DIR}/geobase
${ENSARROOT_SOURCE_DIR}/parbase
${ENSARROOT_SOURCE_DIR}/sofia/mcstack
${ENSARROOT_SOURCE_DIR}/sofia/plists
)

include_directories( ${INCLUDE_DIRECTORIES})
include_directories(SYSTEM ${SYSTEM_INCLUDE_DIRECTORIES})

set(LINK_DIRECTORIES
${ROOT_LIBRARY_DIR}
${FAIRROOT_LIBRARY_DIR}
) 

set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2")
set(CMAKE_CXX_FLAGS_RELEASE        "-O2")
set(CMAKE_CXX_FLAGS_DEBUG          "-g -O2 -fno-reorder-blocks -fno-schedule-insns -fno-inline")
set(CMAKE_CXX_FLAGS_DEBUG          "-g")

link_directories( ${LINK_DIRECTORIES})

set(SOFIAPLIST_SRCS
#EmBinaryCascadeBuilder.cxx
#EmGammaNucleusBuilder.cxx
#EmHadronElasticBuilder.cxx
#EmIonBinaryCascadeBuilder.cxx
G4EmHadronBuilder.cxx
G4EmMuonBuilder.cxx
G4EmQEDBuilder.cxx
G4LowEnergyQEDBuilder.cxx
G4PenelopeQEDBuilder.cxx
G4StepLimiterBuilder.cxx
G4StepLimiterMessenger.cxx
G4StepLimiterPerRegion.cxx
R3BDecaysBuilder.cxx
R3BParticlesBuilder.cxx
R3BPhysicsList.cxx
R3BPhysicsListMessenger.cxx
R3BRunConfiguration.cxx  
)

# fill list of header files from list of source files
# by exchanging the file extension
CHANGE_FILE_EXTENSION(*.cxx *.h SOFIAPLIST_HEADERS "${SOFIAPLIST_SRCS}")

set(SOFIAPLIST_DICT_HEADERS R3BRunConfiguration.h)
set(SOFIAPLIST_LINKDEF PlistLinkDef.h)
set(SOFIAPLIST_DICTIONARY ${CMAKE_CURRENT_BINARY_DIR}/SOFIAPlistDict.cxx)

ROOT_GENERATE_DICTIONARY("${SOFIAPLIST_DICT_HEADERS}" "${SOFIAPLIST_LINKDEF}" "${SOFIAPLIST_DICTIONARY}" "${INCLUDE_DIRECTORIES}")

set(SOFIAPLIST_SRCS ${SOFIAPLIST_SRCS} ${SOFIAPLIST_DICTIONARY})

add_library(SOFIAPlist SHARED ${SOFIAPLIST_SRCS})
target_link_libraries(SOFIAPlist ${ROOT_LIBRARIES})
set_target_properties(SOFIAPlist PROPERTIES 
${FAIRROOT_LIBRARY_PROPERTIES})

################ install ###################
install(TARGETS SOFIAPlist DESTINATION ${CMAKE_BINARY_DIR}/lib)
