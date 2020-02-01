# Indicate source files and dependencies in the files
SET(SRC_demo_nasch_pbc_wb_loop_over_bumps demo_nasch_pbcwb_loop_over_bumps.cpp)

# Create executable (check whether compilation was requested or not)
IF(${CELLULAR_AUTOMATON_BUILD_DEMOS} STREQUAL TRUE)
  ADD_EXECUTABLE(demo_nasch_pbc_wb_loop_over_bumps ${SRC_demo_nasch_pbc_wb_loop_over_bumps})
ELSE(${CELLULAR_AUTOMATON_BUILD_DEMOS} STREQUAL TRUE)
  ADD_EXECUTABLE(demo_nasch_pbc_wb_loop_over_bumps EXCLUDE_FROM_ALL ${SRC_demo_nasch_pbc_wb_loop_over_bumps})
ENDIF(${CELLULAR_AUTOMATON_BUILD_DEMOS} STREQUAL TRUE)

# Indicate linking libraries
SET(LIB_demo_nasch_pbc_wb_loop_over_bumps nasch_lib argparse_lib general_lib)

# ... and link againts them  
TARGET_LINK_LIBRARIES(demo_nasch_pbc_wb_loop_over_bumps ${LIB_demo_nasch_pbc_wb_loop_over_bumps})

# Check if the output/bin directory exists
IF(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/bin)
  # Then create the directory
  FILE(MAKE_DIRECTORY "${bin}")
ENDIF(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/bin)

# Set directory where to create the executables
set_target_properties( demo_nasch_pbc_wb_loop_over_bumps
  PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin"
  )

# ===========================================
# Tests section
# ===========================================
# Run the application to check it works
ADD_TEST(NAME TEST_demo_nasch_pbc_wb_loop_over_bumps_run
          COMMAND demo_nasch_pbc_wb_loop_over_bumps --vmax 4 --bp 0.1 --n_bumps_min 1 --n_bumps_max 10 --bumps_step 1)
IF (CELLULAR_AUTOMATA_USES_DOUBLE_PRECISION)
   SET (VALIDATE_FILENAME_demo_nasch_pbc_wb_loop_over_bumps "validate_double_demo_nasch_pbc_wb_loop_over_bumps.dat")
ELSE (CELLULAR_AUTOMATA_USES_DOUBLE_PRECISION)
   SET (VALIDATE_FILENAME_demo_nasch_pbc_wb_loop_over_bumps "validate_demo_nasch_pbc_wb_loop_over_bumps.dat")
ENDIF (CELLULAR_AUTOMATA_USES_DOUBLE_PRECISION)
ADD_TEST(NAME TEST_demo_nasch_pbc_wb_loop_over_bumps_check_output
         COMMAND ${PROJECT_SOURCE_DIR}/tools/fpdiff.py ${CMAKE_CURRENT_SOURCE_DIR}/validate/${VALIDATE_FILENAME_demo_nasch_pbc_wb_loop_over_bumps} ${CMAKE_CURRENT_BINARY_DIR}/output_test.dat)

# ===========================================
# Test execution order
# ===========================================
SET_TESTS_PROPERTIES(TEST_demo_nasch_pbc_wb_loop_over_bumps_check_output PROPERTIES DEPENDS TEST_demo_nasch_pbc_wb_loop_over_bumps_run)