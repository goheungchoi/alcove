cmake_minimum_required(VERSION 3.25)

project(alcove  VERSION     0.1.0
                LANGUAGES   C CXX
                DESCRIPTION "Vulkan Real-time Rendering Engine"
)

### CMake Debugging Packages  ###
include(CMakePrintHelpers)

#### Setup External Packages ####
find_package(Vulkan REQUIRED)

cmake_print_variables(Vulkan_VERSION)

######## Testing Option  ########

# Prevent any testing conflict 
# when this project is included 
# as a part of a large project
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
  set(IS_ROOT_PROJECT TRUE)
else()
  set(IS_ROOT_PROJECT FALSE)
endif()

if (IS_ROOT_PROJECT) 
  include(CTest)
endif()

# Add Tests Directory
if (IS_ROOT_PROJECT AND BUILD_TESTING)
  message(WARNING "Testing is enabled.")
  add_subdirectory(tests)
endif()
