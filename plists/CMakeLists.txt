# Create a library called "libPlist" which includes the source files
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
${ENSARROOT_SOURCE_DIR}/mcstack
${ENSARROOT_SOURCE_DIR}/passive
${ENSARROOT_SOURCE_DIR}/plists
${ENSARROOT_SOURCE_DIR}/tof
)

include_directories( ${INCLUDE_DIRECTORIES})
include_directories(SYSTEM ${SYSTEM_INCLUDE_DIRECTORIES})

set(LINK_DIRECTORIES
${ROOT_LIBRARY_DIR}
) 

set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2")
set(CMAKE_CXX_FLAGS_RELEASE        "-O2")
set(CMAKE_CXX_FLAGS_DEBUG          "-g -O2 -fno-reorder-blocks -fno-schedule-insns -fno-inline")
set(CMAKE_CXX_FLAGS_DEBUG          "-g")

link_directories( ${LINK_DIRECTORIES})

set(ENSARPLIST_SRCS
#EmBinaryCascadeBuilder.cxx
#EmGammaNucleusBuilder.cxx
#EmHadronElasticBuilder.cxx
#EmIonBinaryCascadeBuilder.cxx
EnsarDecaysBuilder.cxx
EnsarParticlesBuilder.cxx
EnsarPhysicsList.cxx
EnsarPhysicsListMessenger.cxx
EnsarRunConfiguration.cxx  
G4EmHadronBuilder.cxx
G4EmMuonBuilder.cxx
G4EmQEDBuilder.cxx
G4LowEnergyQEDBuilder.cxx
G4PenelopeQEDBuilder.cxx
G4StepLimiterBuilder.cxx
G4StepLimiterMessenger.cxx
G4StepLimiterPerRegion.cxx
)

# fill list of header files from list of source files
# by exchanging the file extension
CHANGE_FILE_EXTENSION(*.cxx *.h ENSARPLIST_HEADERS "${ENSARPLIST_SRCS}")

set(ENSARPLIST_DICT_HEADERS EnsarRunConfiguration.h)
set(ENSARPLIST_LINKDEF PlistLinkDef.h)
set(ENSARPLIST_DICTIONARY ${CMAKE_CURRENT_BINARY_DIR}/EnsarPlistDict.cxx)

ROOT_GENERATE_DICTIONARY("${ENSARPLIST_DICT_HEADERS}" "${ENSARPLIST_LINKDEF}" "${ENSARPLIST_DICTIONARY}" "${INCLUDE_DIRECTORIES}")

set(ENSARPLIST_SRCS ${ENSARPLIST_SRCS} ${ENSARPLIST_DICTIONARY})

add_library(EnsarPlist SHARED ${ENSARPLIST_SRCS})
target_link_libraries(EnsarPlist ${ROOT_LIBRARIES})
set_target_properties(EnsarPlist PROPERTIES 
${FAIRROOT_LIBRARY_PROPERTIES})

################ install ###################
install(TARGETS EnsarPlist DESTINATION ${CMAKE_BINARY_DIR}/lib)
