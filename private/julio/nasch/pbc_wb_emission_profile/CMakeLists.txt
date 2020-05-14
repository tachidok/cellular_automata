# Indicate source files and dependencies in the files
SET(SRC_demo_nasch_pbcwb_ep demo_nasch_pbcwb_ep.cpp)

# Do not create executable but add them to the 'make' list
ADD_EXECUTABLE(julio_demo_nasch_pbcwb_ep EXCLUDE_FROM_ALL ${SRC_demo_nasch_pbcwb_ep})

# Indicate linking libraries
SET(LIB_demo_nasch_pbcwb_ep nasch_lib argparse_lib general_lib)

# ... and link againts them  
TARGET_LINK_LIBRARIES(julio_demo_nasch_pbcwb_ep ${LIB_demo_nasch_pbcwb_ep})

# Check if the output/bin directory exists
IF(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/bin)
  # Then create the directory
  FILE(MAKE_DIRECTORY "${bin}")
ENDIF(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/bin)

# Set directory where to create the executables
set_target_properties( julio_demo_nasch_pbcwb_ep
  PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin"
  )

