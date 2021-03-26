# Indicate source files and dependencies in the files
SET(SRC_demo_nasch_npbcwb demo_nasch_npbcwb.cpp)

# Do not create executable but add them to the 'make' list
ADD_EXECUTABLE(roberto_demo_nasch_npbcwb EXCLUDE_FROM_ALL ${SRC_demo_nasch_npbcwb})

# Indicate linking libraries
SET(LIB_demo_nasch_npbcwb nasch_lib general_lib)

# ... and link againts them
TARGET_LINK_LIBRARIES(roberto_demo_nasch_npbcwb ${LIB_demo_nasch_npbcwb})

# Check if the output/bin directory exists
IF(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/bin)
  # Then create the directory
  FILE(MAKE_DIRECTORY "${bin}")
ENDIF(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/bin)

# Set directory where to create the executables
set_target_properties( roberto_demo_nasch_npbcwb
  PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin"
  )
