# Check for cmake minimum required version (previously working with v2.8)
CMAKE_MINIMUM_REQUIRED(VERSION 3.5)

# ----------------------------------------------------------------------
# Project intialisation
# ----------------------------------------------------------------------

# Project name (this case library's name)
PROJECT(cellular_automaton)
SET(${CMAKE_PROJECT_NAME}_MAJOR_VERSION 0)
SET(${CMAKE_PROJECT_NAME}_MINOR_VERSION 1)
SET(${CMAKE_PROJECT_NAME}_PATCH_LEVEL 0)

# Prints a lot of stuff
#SET(CMAKE_VERBOSE_MAKEFILE TRUE)

# ----------------------------------------------------------------------
# Output requested configuration
# ----------------------------------------------------------------------
MESSAGE( STATUS "" )
MESSAGE( STATUS "-----------------------------------------------------------------------" )
MESSAGE( STATUS "** GENERAL LIBRARY INFORMATION **" )
MESSAGE( STATUS "-----------------------------------------------------------------------" )
#MESSAGE( STATUS "CELLULAR_AUTOMATON_LIB_TYPE:            " ${CELLULAR_AUTOMATON_LIB_TYPE} )
MESSAGE( STATUS "CELLULAR_AUTOMATON_BUILD_VERSION:       " ${CELLULAR_AUTOMATON_BUILD_VERSION} )
MESSAGE( STATUS "CELLULAR_AUTOMATON_BUILD_DEMOS:         " ${CELLULAR_AUTOMATON_BUILD_DEMOS} )
MESSAGE( STATUS "PROJECT_NAME:                 " ${PROJECT_NAME})
MESSAGE( STATUS "PROJECT_MAJOR_VERSION:        " ${cellular_automaton_MAJOR_VERSION})
MESSAGE( STATUS "PROJECT_MINOR_VERSION:        " ${cellular_automaton_MINOR_VERSION})
MESSAGE( STATUS "PROJECT_PATCH_LEVEL:          " ${cellular_automaton_PATCH_LEVEL})
MESSAGE( STATUS "-----------------------------------------------------------------------" )

# ----------------------------------------------------------------------
# Read configuration file and extract values of variables
# ----------------------------------------------------------------------
MESSAGE( STATUS "" )
MESSAGE( STATUS "-----------------------------------------------------------------------" )
MESSAGE( STATUS "** READ VARIABLES FROM CONFIGURATION FILE: " ${CELLULAR_AUTOMATON_CONFIGURATION_FILE} " **")
MESSAGE( STATUS "-----------------------------------------------------------------------" )
file(STRINGS ${CELLULAR_AUTOMATON_CONFIGURATION_FILE} ConfigContents)
foreach(NameAndValue ${ConfigContents})
  # Strip leading spaces
  string(REGEX REPLACE "^[ ]+" "" NameAndValue ${NameAndValue})
  # Find variable name
  string(REGEX MATCH "^[^=]+" Name ${NameAndValue})
  # Find the value
  string(REPLACE "${Name}=" "" Value ${NameAndValue})
  # Set the variable
  set(${Name} "${Value}")
  MESSAGE( STATUS ${Name} " : " ${Value})
endforeach()

MESSAGE( STATUS "-----------------------------------------------------------------------" )

# ----------------------------------------------------------------------
# Modify compilation variables
# ----------------------------------------------------------------------
IF(${CELLULAR_AUTOMATON_BUILD_VERSION} STREQUAL DEBUG)
  SET(CELLULAR_AUTOMATON_C_FLAGS "-O0 -g -Wall -Wshadow")
  SET(CELLULAR_AUTOMATON_CXX_FLAGS "-O0 -g -Wall -Wshadow -std=c++11")
ELSEIF(${CELLULAR_AUTOMATON_BUILD_VERSION} STREQUAL RELEASE)
  SET(CELLULAR_AUTOMATON_C_FLAGS "-O3 -Wall -Wshadow -DNDEBUG")
  SET(CELLULAR_AUTOMATON_CXX_FLAGS "-O3 -Wall -Wshadow -DNDEBUG -std=c++11")
ELSEIF(${CELLULAR_AUTOMATON_BUILD_VERSION} STREQUAL PROFILING)
  SET(CELLULAR_AUTOMATON_C_FLAGS "-O0 -pg -Wall -Wshadow")
  SET(CELLULAR_AUTOMATON_CXX_FLAGS "-O0 -pg -Wall -Wshadow -std=c++11")
  MESSAGE( WARNING "You are compiling with profiling ENABLED, this will SIGNIFICANTLY increase execution times. If you do not want profiling to be ENABLED then choose another version when asked for library version" ) 
ELSE(${CELLULAR_AUTOMATON_BUILD_VERSION} STREQUAL DEBUG)
  SET(CELLULAR_AUTOMATON_C_FLAGS "-O0 -g -Wall -Wshadow")
  SET(CELLULAR_AUTOMATON_CXX_FLAGS "-O0 -g -Wall -Wshadow -std=c++11")
ENDIF(${CELLULAR_AUTOMATON_BUILD_VERSION} STREQUAL DEBUG)

# ----------------------------------------------------------------------
# Append extra definitions based on configuration files
# ----------------------------------------------------------------------
SET(CELLULAR_AUTOMATON_RANGE_CHECK_DEFINITION "-DCELLULAR_AUTOMATON_RANGE_CHECK")
IF (CELLULAR_AUTOMATON_RANGE_CHECK)
  SET(CELLULAR_AUTOMATON_CXX_FLAGS "${CELLULAR_AUTOMATON_CXX_FLAGS} ${CELLULAR_AUTOMATON_RANGE_CHECK_DEFINITION}" )
ENDIF (CELLULAR_AUTOMATON_RANGE_CHECK)

SET(CELLULAR_AUTOMATON_USES_DOUBLE_PRECISION_DEFINITION "-DTYPEDEF_REAL_IS_DOUBLE")
IF (CELLULAR_AUTOMATON_USES_DOUBLE_PRECISION)
  SET(CELLULAR_AUTOMATON_CXX_FLAGS "${CELLULAR_AUTOMATON_CXX_FLAGS} ${CELLULAR_AUTOMATON_USES_DOUBLE_PRECISION_DEFINITION}" )
ENDIF (CELLULAR_AUTOMATON_USES_DOUBLE_PRECISION)

SET(CELLULAR_AUTOMATON_PANIC_MODE_DEFINE "-DCELLULAR_AUTOMATON_PANIC_MODE")
IF (CELLULAR_AUTOMATON_PANIC_MODE)
  SET(CELLULAR_AUTOMATON_CXX_FLAGS "${CELLULAR_AUTOMATON_CXX_FLAGS} ${CELLULAR_AUTOMATON_PANIC_MODE_DEFINE}" )
ENDIF (CELLULAR_AUTOMATON_PANIC_MODE)

# ----------------------------------------------------------------------
# Set modified compilation variables
# ----------------------------------------------------------------------
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${CELLULAR_AUTOMATON_C_FLAGS}" )
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CELLULAR_AUTOMATON_CXX_FLAGS}" )

# Uncomment this line if you added extra libraries
#SET(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} ${GCC_COVERAGE_LINK_FLAGS}" )

# ----------------------------------------------------------------------
# Include testing (this function already calls enable_testing())
# ----------------------------------------------------------------------
INCLUDE(CTest)

# ----------------------------------------------------------------------
# Output configuration
# ----------------------------------------------------------------------
MESSAGE( STATUS "" )
MESSAGE( STATUS "-----------------------------------------------------------------------" )
MESSAGE( STATUS "** GENERAL COMPILATION FLAGS **" )
MESSAGE( STATUS "-----------------------------------------------------------------------" )
# Output compilers versions
MESSAGE( STATUS "CMAKE_C_COMPILER:             " ${CMAKE_C_COMPILER} )
MESSAGE( STATUS "CMAKE_CXX_COMPILER:           " ${CMAKE_CXX_COMPILER} )

# Output configuration flags
# The flags passed to the C-compiler
MESSAGE( STATUS "CMAKE_C_FLAGS:                " ${CMAKE_C_FLAGS} )
# The flags passed to the C++ compiler
MESSAGE( STATUS "CMAKE_CXX_FLAGS:              " ${CMAKE_CXX_FLAGS} )
MESSAGE( STATUS "CMAKE_EXE_LINKER_FLAGS:       " ${CMAKE_EXE_LINKER_FLAGS} )

# Output some extra info.
MESSAGE( STATUS "CMAKE_VERBOSE_MAKEFILE:       " ${CMAKE_VERBOSE_MAKEFILE} )

MESSAGE( STATUS "-----------------------------------------------------------------------" )

# Add external source files sub-directories
ADD_SUBDIRECTORY(external_src)

# Add sub-directories with source files
ADD_SUBDIRECTORY(src)

# Add private folders (note that any source code in here is not
# compiled but only added to the make list)
ADD_SUBDIRECTORY(private)

# Once all source code has been compiled add sub-directories for demos
ADD_SUBDIRECTORY(demos)

MESSAGE( STATUS "" )
MESSAGE( STATUS "-----------------------------------------------------------------------" )
MESSAGE( STATUS "-----------------------------------------------------------------------" )
MESSAGE( STATUS "" )

