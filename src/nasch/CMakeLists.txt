# Add source files in order of dependence, the ones with no dependency
# first then the others
SET(BASE_SRC_FILES cc_nasch_pbc.cpp cc_nasch_npbc.cpp)

SET(SRC_FILES ${BASE_SRC_FILES})

# Include current directory to the path
#INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

# Create a library with the above files based on the requested library
# version
IF(${CELLULAR_AUTOMATON_LIB_TYPE} STREQUAL STATIC)
  ADD_LIBRARY(nasch_lib STATIC ${SRC_FILES})
ELSEIF(${CELLULAR_AUTOMATON_LIB_TYPE} STREQUAL SHARED)
 ADD_LIBRARY(nasch_lib SHARED ${SRC_FILES})
ELSE(${CELLULAR_AUTOMATON_LIB_TYPE} STREQUAL STATIC)
 ADD_LIBRARY(nasch_lib ${SRC_FILES})
ENDIF(${CELLULAR_AUTOMATON_LIB_TYPE} STREQUAL STATIC)

# Indicate dependencies within libraries (it seems not to be necessary)
#TARGET_LINK_LIBRARIES(matrices_lib general_lib)

# Now make the library available for its use
#TARGET_INCLUDE_DIRECTORIES(matrices ${CMAKE_CURRENT_SOURCE_DIR})
