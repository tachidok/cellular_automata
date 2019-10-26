# Indicate source files and dependencies in the files
SET(SRC_demo_nasch_npbc demo_nasch_npbc.cpp)

# Do not create executable but add them to the 'make' list
ADD_EXECUTABLE(julio_demo_nasch_npbc EXCLUDE_FROM_ALL ${SRC_demo_nasch_npbc})

# Indicate linking libraries
SET(LIB_demo_nasch_npbc nasch_lib general_lib)

# ... and link againts them  
TARGET_LINK_LIBRARIES(julio_demo_nasch_npbc ${LIB_demo_nasch_npbc})

# Check if the output/bin directory exists
IF(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/bin)
  # Then create the directory
  FILE(MAKE_DIRECTORY "${bin}")
ENDIF(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/bin)

# Set directory where to create the executables
set_target_properties( julio_demo_nasch_npbc
  PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin"
  )

