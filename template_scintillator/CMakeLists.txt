# Create a library called "libEnsarScintillator" which includes the source files given in
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
${ENSARROOT_SOURCE_DIR}/template_scintillator/
${ENSARROOT_SOURCE_DIR}/template_scintillator/geometry
${ENSARROOT_SOURCE_DIR}/template_scintillator/digitization
)

include_directories( ${INCLUDE_DIRECTORIES})
include_directories(SYSTEM ${SYSTEM_INCLUDE_DIRECTORIES})

set(LINK_DIRECTORIES
${ROOT_LIBRARY_DIR}
${FAIRROOT_LIBRARY_DIR}
) 

link_directories( ${LINK_DIRECTORIES})

set(TSCINTIDET_SRCS
# --- geometry ---
geometry/EnsarScintillator.cxx
geometry/EnsarScintillatorContFact.cxx
geometry/EnsarScintillatorGeoPar.cxx
# --- digitization ---
digitization/EnsarScintillatorDigi.cxx
digitization/EnsarScintillatorDigiPar.cxx
digitization/EnsarScintillatorPoint.cxx
digitization/EnsarScintillatorDigitizer.cxx
)

# fill list of header files from list of source files
# by exchanging the file extension
CHANGE_FILE_EXTENSION(*.cxx *.h TSCINTIDET_HEADERS "${TSCINTIDET_SRCS}")

set(TSCINTIDET_LINKDEF EnsarScintillatorLinkDef.h)
set(TSCINTIDET_DICTIONARY ${CMAKE_CURRENT_BINARY_DIR}/EnsarScintillatorDict.cxx) 

ROOT_GENERATE_DICTIONARY("${TSCINTIDET_HEADERS}" "${TSCINTIDET_LINKDEF}" "${TSCINTIDET_DICTIONARY}" "${INCLUDE_DIRECTORIES}")

set(TSCINTIDET_SRCS ${TSCINTIDET_SRCS} ${TSCINTIDET_DICTIONARY})

add_library(EnsarScintillator SHARED ${TSCINTIDET_SRCS})
target_link_libraries(EnsarScintillator ${ROOT_LIBRARIES})
#set_target_properties(EnsarScintillator PROPERTIES  ${ENSARROOT_LIBRARY_PROPERTIES})

################ install ###################
install(TARGETS EnsarScintillator DESTINATION ${CMAKE_BINARY_DIR}/lib)
