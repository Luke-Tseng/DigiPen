# Create a variable called MODULE that contains the name of the module
set (MODULE MemDebugger)

# Collect sources into the variable SRC_FILES with 
# having to explicitly list each header and source file
file (GLOB SRC_FILES 
      "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h"
      "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
      "${CMAKE_CURRENT_SOURCE_DIR}/*.c"
      "${CMAKE_CURRENT_SOURCE_DIR}/*.cc"
      "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
      "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp"
)

# Set Properties->General->Configuration Type to Dynamic Library(.dll)
# Creates math.dll with the listed sources collected in the variables
# Also adds sources to the Solution Explorer
add_library(${MODULE} STATIC ${SRC_FILES})

# Properties->C/C++->General->Additional Include Directories
# state that anybody linking to us needs to include the current source dir
# to find the public .h files, while we don't.
target_include_directories(${MODULE} INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_include_directories(${MODULE} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}/include")

# Creates a folder "libraries" and adds target project (math.vcproj) under it
set_property(TARGET ${MODULE} PROPERTY FOLDER "libraries")

# Properties->General->Output Directory
# set_target_properties(${MODULE} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)

# Adds logic to INSTALL.vcproj to copy ${MODULE}.lib to the destination directory
install (TARGETS ${MODULE}
      RUNTIME DESTINATION ${PROJECT_SOURCE_DIR}/_install
      LIBRARY DESTINATION ${PROJECT_SOURCE_DIR}/_install)
