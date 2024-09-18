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
list(PREPEND CMAKE_PREFIX_PATH 
	"$ENV{VULKAN_SDK}/Lib/cmake"
	"$ENV{VULKAN_SDK}/Lib/cmake/glslang"
)
list(PREPEND CMAKE_MODULE_PATH 
	"$ENV{VULKAN_SDK}/Lib/cmake"
	"$ENV{VULKAN_SDK}/Lib/cmake/glslang"
)
cmake_print_variables(CMAKE_MODULE_PATH)
#########################################

#### Setup External Packages ############
# Vulkan Provided
find_package(Vulkan REQUIRED 
  COMPONENTS 
  glslangValidator
  glslang 
  SPIRV-Tools
)

#########################################
#########################################
#########################################

include("cmake-modules/FindVulkanExtra.cmake")
find_vulkan_extra()

include("cmake-modules/CompileGLSLShaders.cmake")
compile_glsl_shaders("${CMAKE_CURRENT_SOURCE_DIR}/shaders")

include("cmake-modules/CopyFilesByExtension.cmake")
copy_files_by_extension("${CMAKE_CURRENT_SOURCE_DIR}/shaders" "${CMAKE_BINARY_DIR}/shaders" "spv")

# Make sure that the copy operation is preceded by the compile operation
add_dependencies(CopyFileByExtension CompileShader)

#########################################
#########################################
#########################################


# Conan Provided Packages

# SDL2
find_package(SDL2 REQUIRED CONFIG)
cmake_print_variables(SDL2_VERSION)
# fmt
find_package(fmt REQUIRED)
cmake_print_variables(fmt_VERSION)
# GLM
find_package(glm REQUIRED)
cmake_print_variables(glm_VERSION)
# Dear-imgui
find_package(imgui REQUIRED)
cmake_print_variables(imgui_VERSION)
set(imgui_BACKEND_DIR "${CMAKE_CURRENT_SOURCE_DIR}/extern/imgui/backends")
# Assimp
find_package(assimp REQUIRED)
cmake_print_variables(assimp_VERSION)

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
