cmake_minimum_required(VERSION 3.25)

project(alcove  VERSION     0.1.0
                DESCRIPTION "Vulkan Real-time Rendering Engine"
                LANGUAGES   C CXX
)

### CMake Debugging Packages  ###
include(CMakePrintHelpers)
############### Config ##################
set(Alcove_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(Alcove_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include/alcove")
cmake_print_variables(Alcove_SOURCE_DIR)
cmake_print_variables(Alcove_INCLUDE_DIR)
cmake_print_variables(CMAKE_SOURCE_DIR)
cmake_print_variables(CMAKE_BINARY_DIR)
#########################################
# list(PREPEND CMAKE_PREFIX_PATH "C:/VulkanSDK/1.3.268.0/cmake")
# cmake_print_variables(CMAKE_MODULE_PATH)
#########################################

#### Setup External Packages ####
# Vulkan Provided
find_package(Vulkan REQUIRED 
  COMPONENTS 
  glslang 
  SPIRV-Tools
)

#########################################
#########################################
#########################################
if (WIN32) 
  if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(Vulkan_BINARY_DIR "$ENV{VULKAN_SDK}/Bin")
    set(Vulkan_LIBRARY_DIR "$ENV{VULKAN_SDK}/Lib")
  else()
    set(Vulkan_BINARY_DIR "$ENV{VULKAN_SDK}/Bin32")
    set(Vulkan_LIBRARY_DIR "$ENV{VULKAN_SDK}/Lib32")
  endif()
else()
  set(Vulkan_BINARY_DIR "$ENV{VULKAN_SDK}/bin")
  set(Vulkan_LIBRARY_DIR "$ENV{VULKAN_SDK}/lib")
endif()

find_library(
  Vulkan_glslang-default-resource-limits_LIBRARY
  NAMES glslang-default-resource-limits
  HINTS ${Vulkan_LIBRARY_DIR}
)
mark_as_advanced(Vulkan_glslang-default-resource-limits_LIBRARY)
find_library(
  Vulkan_glslang-default-resource-limits_DEBUG_LIBRARY
  NAMES glslang-default-resource-limitsd
  HINTS ${Vulkan_LIBRARY_DIR}
)
mark_as_advanced(Vulkan_glslang-default-resource-limits_DEBUG_LIBRARY)

set(glslang-default-resource-limits_lib 
  "$<$<CONFIG:Debug>:${Vulkan_glslang-default-resource-limits_DEBUG_LIBRARY}>$<$<NOT:$<CONFIG:Debug>>:${Vulkan_glslang-default-resource-limits_LIBRARY}>"
)

find_library(
  Vulkan_SPIRV-Tools-opt_LIBRARY
  NAMES SPIRV-Tools-opt
  HINTS ${Vulkan_LIBRARY_DIR}
)
mark_as_advanced(Vulkan_SPIRV-Tools-opt_LIBRARY)
find_library(
  Vulkan_SPIRV-Tools-opt_DEBUG_LIBRARY
  NAMES SPIRV-Tools-optd
  HINTS ${Vulkan_LIBRARY_DIR}
)
mark_as_advanced(Vulkan_SPIRV-Tools-opt_DEBUG_LIBRARY)

set(Vulkan_SPIRV-Tools-opt 
  "$<$<CONFIG:Debug>:${Vulkan_SPIRV-Tools-opt_DEBUG_LIBRARY}>$<$<NOT:$<CONFIG:Debug>>:${Vulkan_SPIRV-Tools-opt_LIBRARY}>"
)
#########################################
#########################################
#########################################
cmake_print_variables(Vulkan_VERSION)
cmake_print_variables(Vulkan_glslang_FOUND)
cmake_print_variables(Vulkan_SPIRV-Tools_FOUND)
cmake_print_variables(Vulkan_INCLUDE_DIRS)


# Conan Provided
find_package(SDL2 REQUIRED CONFIG)
cmake_print_variables(SDL2_VERSION)
find_package(fmt REQUIRED)
cmake_print_variables(fmt_VERSION)
find_package(glm REQUIRED)
cmake_print_variables(glm_VERSION)
find_package(imgui REQUIRED)
cmake_print_variables(imgui_VERSION)
find_package(vulkan-memory-allocator REQUIRED)
cmake_print_variables(vulkan-memory-allocator_VERSION)

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

set(Vulkan_INCLUDE_DIR "$ENV{VULKAN_SDK}/Include")
set(Vulkan_INCLUDE_DIRS "$ENV{VULKAN_SDK}/Include")

# Add Source Directory
add_subdirectory(src)

# Add Executable Directory
add_subdirectory(app)

# Add Tests Directory
if (IS_ROOT_PROJECT AND BUILD_TESTING)
  message(WARNING "Testing is enabled.")
  add_subdirectory(tests)
endif()
