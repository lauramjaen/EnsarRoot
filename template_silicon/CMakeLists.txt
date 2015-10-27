# Create a library called "libEnsarSilicon" which includes the source files given in
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
${ENSARROOT_SOURCE_DIR}/fairtools
${ENSARROOT_SOURCE_DIR}/base
${ENSARROOT_SOURCE_DIR}/parbase
${ENSARROOT_SOURCE_DIR}/geobase
${ENSARROOT_SOURCE_DIR}/ensarbase
${ENSARROOT_SOURCE_DIR}/ensardata
${ENSARROOT_SOURCE_DIR}/ensargen
${ENSARROOT_SOURCE_DIR}/mcstack
${ENSARROOT_SOURCE_DIR}/passive
${ENSARROOT_SOURCE_DIR}/template_silicon/
${ENSARROOT_SOURCE_DIR}/template_silicon/geometry
${ENSARROOT_SOURCE_DIR}/template_silicon/digitization
)

include_directories( ${INCLUDE_DIRECTORIES})
include_directories(SYSTEM ${SYSTEM_INCLUDE_DIRECTORIES})

set(LINK_DIRECTORIES
${ROOT_LIBRARY_DIR}
${FAIRROOT_LIBRARY_DIR}
) 

link_directories( ${LINK_DIRECTORIES})

set(TSILIDET_SRCS
# --- geometry ---
geometry/EnsarSilicon.cxx
geometry/EnsarSiliconContFact.cxx
geometry/EnsarSiliconGeoPar.cxx
# --- digitization ---
digitization/EnsarSiliconDigi.cxx
digitization/EnsarSiliconDigiPar.cxx
digitization/EnsarSiliconPoint.cxx
digitization/EnsarSiliconDigitizer.cxx
)

# fill list of header files from list of source files
# by exchanging the file extension
CHANGE_FILE_EXTENSION(*.cxx *.h TSILIDET_HEADERS "${TSILIDET_SRCS}")

set(TSILIDET_LINKDEF EnsarSiliconLinkDef.h)
set(TSILIDET_DICTIONARY ${CMAKE_CURRENT_BINARY_DIR}/EnsarSiliconDict.cxx) 

ROOT_GENERATE_DICTIONARY("${TSILIDET_HEADERS}" "${TSILIDET_LINKDEF}" "${TSILIDET_DICTIONARY}" "${INCLUDE_DIRECTORIES}")

set(TSILIDET_SRCS ${TSILIDET_SRCS} ${TSILIDET_DICTIONARY})

add_library(EnsarSilicon SHARED ${TSILIDET_SRCS})
target_link_libraries(EnsarSilicon ${ROOT_LIBRARIES})
#set_target_properties(EnsarSilicon PROPERTIES  ${ENSARROOT_LIBRARY_PROPERTIES})

################ install ###################
install(TARGETS EnsarSilicon DESTINATION ${CMAKE_BINARY_DIR}/lib)
