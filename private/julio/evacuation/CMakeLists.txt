# Indicate source files and dependencies in the files
SET(SRC_demo_evacuation demo_evacuation.cpp)

# Do not create executable but add them to the 'make' list
ADD_EXECUTABLE(julio_demo_evacuation EXCLUDE_FROM_ALL ${SRC_demo_evacuation})

# Indicate linking libraries
SET(LIB_demo_evacuation argparse_lib general_lib)

# ... and link againts them  
TARGET_LINK_LIBRARIES(julio_demo_evacuation ${LIB_demo_evacuation})

# Check if the output/bin directory exists
IF(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/bin)
  # Then create the directory
  FILE(MAKE_DIRECTORY "${bin}")
ENDIF(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/bin)

# Set directory where to create the executables
set_target_properties( julio_demo_evacuation
  PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin"
  )

