# Indicate source files and dependencies in the files
SET(SRC_demo_kkw_pbcwb demo_kkw_pbcwb.cpp)

# Do not create executable but add them to the 'make' list
ADD_EXECUTABLE(julio_demo_kkw_pbcwb EXCLUDE_FROM_ALL ${SRC_demo_kkw_pbcwb})

# Indicate linking libraries
SET(LIB_demo_kkw_pbcwb kkw_lib argparse_lib general_lib)

# ... and link againts them  
TARGET_LINK_LIBRARIES(julio_demo_kkw_pbcwb ${LIB_demo_kkw_pbcwb})

# Check if the output/bin directory exists
IF(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/bin)
  # Then create the directory
  FILE(MAKE_DIRECTORY "${bin}")
ENDIF(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/bin)

# Set directory where to create the executables
set_target_properties( julio_demo_kkw_pbcwb
  PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin"
  )

