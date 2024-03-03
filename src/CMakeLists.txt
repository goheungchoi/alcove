set(LIBRARY_NAME "fact")

set(  # Define the source files
  LIBRARY_SOURCES 
  "fact.cpp"
)

set(  # Define the header files
  LIBRARY_HEADERS 
  "fact.h"
)

set(  # Define Library include paths
  LIBRARY_INCLUDES
  "./"
)

add_library(${LIBRARY_NAME} STATIC ${LIBRARY_SOURCES} ${LIBRARY_HEADERS})  # Add the Library target
target_include_directories(${LIBRARY_NAME} PUBLIC ${LIBRARY_INCLUDES})


##########################################
###### Add Subdirectories ################
##########################################
add_subdirectory(core)


##########################################
##########################################