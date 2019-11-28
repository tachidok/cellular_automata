# Indicate source files and dependencies in the files
SET(SRC_demo_argparse demo_argparse.cpp)

# Create executable (check whether compilation was requested or not)
IF(${CELLULAR_AUTOMATON_BUILD_DEMOS} STREQUAL TRUE)
  ADD_EXECUTABLE(demo_argparse ${SRC_demo_argparse})
ELSE(${CELLULAR_AUTOMATON_BUILD_DEMOS} STREQUAL TRUE)
  ADD_EXECUTABLE(demo_argparse EXCLUDE_FROM_ALL ${SRC_demo_argparse})
ENDIF(${CELLULAR_AUTOMATON_BUILD_DEMOS} STREQUAL TRUE)

# Indicate linking libraries
SET(LIB_demo_argparse argparse_lib general_lib)

# ... and link againts them  
TARGET_LINK_LIBRARIES(demo_argparse ${LIB_demo_argparse})

# Check if the output/bin directory exists
IF(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/bin)
  # Then create the directory
  FILE(MAKE_DIRECTORY "${bin}")
ENDIF(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/bin)

# Set directory where to create the executables
set_target_properties( demo_argparse
  PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin"
  )

# ===========================================
# Tests section
# ===========================================
# Run the application to check it works
ADD_TEST(NAME TEST_demo_argparse_run
          COMMAND demo_argparse)
IF (CHAPCHOM_USES_DOUBLE_PRECISION)
   SET (VALIDATE_FILENAME_demo_argparse "validate_double_demo_argparse.dat")
ELSE (CHAPCHOM_USES_DOUBLE_PRECISION)
     SET (VALIDATE_FILENAME_demo_argparse "validate_demo_argparse.dat")
ENDIF (CHAPCHOM_USES_DOUBLE_PRECISION)
ADD_TEST(NAME TEST_demo_argparse_check_output
         COMMAND ${PROJECT_SOURCE_DIR}/tools/fpdiff.py ${CMAKE_CURRENT_SOURCE_DIR}/validate/${VALIDATE_FILENAME_demo_argparse} ${CMAKE_CURRENT_BINARY_DIR}/output_test.dat)