# Indicate source files and dependencies in the files
SET(SRC_demo_nasch_npbcwcw demo_nasch_npbcwcw.cpp)

# Do not create executable but add them to the 'make' list
ADD_EXECUTABLE(roberto_demo_nasch_npbcwcw EXCLUDE_FROM_ALL ${SRC_demo_nasch_npbcwcw})

# Indicate linking libraries
SET(LIB_demo_nasch_npbcwcw nasch_lib crosswalk_lib argparse_lib general_lib)

# ... and link againts them
TARGET_LINK_LIBRARIES(roberto_demo_nasch_npbcwcw ${LIB_demo_nasch_npbcwcw})

# Check if the output/bin directory exists
IF(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/bin)
  # Then create the directory
  FILE(MAKE_DIRECTORY "${bin}")
ENDIF(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/bin)

# Set directory where to create the executables
set_target_properties( roberto_demo_nasch_npbcwcw
  PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin"
  )
