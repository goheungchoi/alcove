

# Conan automatically generated toolchain file
# DO NOT EDIT MANUALLY, it will be overwritten

# Avoid including toolchain file several times (bad if appending to variables like
#   CMAKE_CXX_FLAGS. See https://github.com/android/ndk/issues/323
include_guard()

message(STATUS "Using Conan toolchain: ${CMAKE_CURRENT_LIST_FILE}")

if(${CMAKE_VERSION} VERSION_LESS "3.15")
    message(FATAL_ERROR "The 'CMakeToolchain' generator only works with CMake >= 3.15")
endif()




########## generic_system block #############
# Definition of system, platform and toolset
#############################################


set(CMAKE_GENERATOR_PLATFORM "x64" CACHE STRING "" FORCE)

message(STATUS "Conan toolchain: CMAKE_GENERATOR_TOOLSET=v143")
set(CMAKE_GENERATOR_TOOLSET "v143" CACHE STRING "" FORCE)






# Definition of VS runtime, defined from build_type, compiler.runtime, compiler.runtime_type
cmake_policy(GET CMP0091 POLICY_CMP0091)
if(NOT "${POLICY_CMP0091}" STREQUAL NEW)
    message(FATAL_ERROR "The CMake policy CMP0091 must be NEW, but is '${POLICY_CMP0091}'")
endif()
set(CMAKE_MSVC_RUNTIME_LIBRARY "$<$<CONFIG:Debug>:MultiThreadedDebugDLL>")

message(STATUS "Conan toolchain: C++ Standard 20 with extensions OFF")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

string(APPEND CONAN_CXX_FLAGS " /MP24")
string(APPEND CONAN_C_FLAGS " /MP24")

# Extra c, cxx, linkflags and defines


if(DEFINED CONAN_CXX_FLAGS)
  string(APPEND CMAKE_CXX_FLAGS_INIT " ${CONAN_CXX_FLAGS}")
endif()
if(DEFINED CONAN_C_FLAGS)
  string(APPEND CMAKE_C_FLAGS_INIT " ${CONAN_C_FLAGS}")
endif()
if(DEFINED CONAN_SHARED_LINKER_FLAGS)
  string(APPEND CMAKE_SHARED_LINKER_FLAGS_INIT " ${CONAN_SHARED_LINKER_FLAGS}")
endif()
if(DEFINED CONAN_EXE_LINKER_FLAGS)
  string(APPEND CMAKE_EXE_LINKER_FLAGS_INIT " ${CONAN_EXE_LINKER_FLAGS}")
endif()

get_property( _CMAKE_IN_TRY_COMPILE GLOBAL PROPERTY IN_TRY_COMPILE )
if(_CMAKE_IN_TRY_COMPILE)
    message(STATUS "Running toolchain IN_TRY_COMPILE")
    return()
endif()

set(CMAKE_FIND_PACKAGE_PREFER_CONFIG ON)

# Definition of CMAKE_MODULE_PATH
list(PREPEND CMAKE_MODULE_PATH "C:/Users/ga1ox/.conan2/p/b/catchac1bb3a62a527/p/lib/cmake/Catch2")
# the generators folder (where conan generates files, like this toolchain)
list(PREPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR})

# Definition of CMAKE_PREFIX_PATH, CMAKE_XXXXX_PATH
# The explicitly defined "builddirs" of "host" context dependencies must be in PREFIX_PATH
list(PREPEND CMAKE_PREFIX_PATH "C:/Users/ga1ox/.conan2/p/b/catchac1bb3a62a527/p/lib/cmake/Catch2")
# The Conan local "generators" folder, where this toolchain is saved.
list(PREPEND CMAKE_PREFIX_PATH ${CMAKE_CURRENT_LIST_DIR} )
list(PREPEND CMAKE_LIBRARY_PATH "C:/Users/ga1ox/.conan2/p/b/sdl78d843de900ef/p/lib" "C:/Users/ga1ox/.conan2/p/b/catchac1bb3a62a527/p/lib" "C:/Users/ga1ox/.conan2/p/b/imgui0ff22e947c79b/p/lib" "C:/Users/ga1ox/.conan2/p/b/fmt8b17a52e0b4c3/p/lib" "C:/Users/ga1ox/.conan2/p/b/assim780911c60acc0/p/lib" "C:/Users/ga1ox/.conan2/p/b/miniz9ea3a5070eef7/p/lib" "C:/Users/ga1ox/.conan2/p/b/bzip2aff2ffdca58fa/p/lib" "C:/Users/ga1ox/.conan2/p/b/pugixc66baebf36ad5/p/lib" "C:/Users/ga1ox/.conan2/p/b/zlib6d3badaaccfc5/p/lib" "C:/Users/ga1ox/.conan2/p/b/kuba-ce1ac7d8fe74f/p/lib" "C:/Users/ga1ox/.conan2/p/b/poly26c25e8156ab93/p/lib" "C:/Users/ga1ox/.conan2/p/b/draco9e9b07265ad3c/p/lib" "C:/Users/ga1ox/.conan2/p/b/clipp05743da8e6f52/p/lib" "C:/Users/ga1ox/.conan2/p/b/openda4531ef8a3b15/p/lib")
list(PREPEND CMAKE_INCLUDE_PATH "C:/Users/ga1ox/.conan2/p/glm67cb345896424/p/include" "C:/Users/ga1ox/.conan2/p/b/sdl78d843de900ef/p/include" "C:/Users/ga1ox/.conan2/p/b/sdl78d843de900ef/p/include/SDL2" "C:/Users/ga1ox/.conan2/p/b/catchac1bb3a62a527/p/include" "C:/Users/ga1ox/.conan2/p/b/imgui0ff22e947c79b/p/include" "C:/Users/ga1ox/.conan2/p/b/fmt8b17a52e0b4c3/p/include" "C:/Users/ga1ox/.conan2/p/b/assim780911c60acc0/p/include" "C:/Users/ga1ox/.conan2/p/b/miniz9ea3a5070eef7/p/include" "C:/Users/ga1ox/.conan2/p/b/miniz9ea3a5070eef7/p/include/minizip" "C:/Users/ga1ox/.conan2/p/b/bzip2aff2ffdca58fa/p/include" "C:/Users/ga1ox/.conan2/p/b/pugixc66baebf36ad5/p/include" "C:/Users/ga1ox/.conan2/p/utfcpa9d0ce6f8606f/p/include" "C:/Users/ga1ox/.conan2/p/utfcpa9d0ce6f8606f/p/include/utf8cpp" "C:/Users/ga1ox/.conan2/p/b/zlib6d3badaaccfc5/p/include" "C:/Users/ga1ox/.conan2/p/b/kuba-ce1ac7d8fe74f/p/include" "C:/Users/ga1ox/.conan2/p/b/poly26c25e8156ab93/p/include" "C:/Users/ga1ox/.conan2/p/rapidf7a3355ba53c4/p/include" "C:/Users/ga1ox/.conan2/p/b/draco9e9b07265ad3c/p/include" "C:/Users/ga1ox/.conan2/p/b/clipp05743da8e6f52/p/include" "C:/Users/ga1ox/.conan2/p/stb6342cecb318f5/p/include" "C:/Users/ga1ox/.conan2/p/b/openda4531ef8a3b15/p/include")



if (DEFINED ENV{PKG_CONFIG_PATH})
set(ENV{PKG_CONFIG_PATH} "${CMAKE_CURRENT_LIST_DIR};$ENV{PKG_CONFIG_PATH}")
else()
set(ENV{PKG_CONFIG_PATH} "${CMAKE_CURRENT_LIST_DIR};")
endif()




message(STATUS "Conan toolchain: Setting BUILD_SHARED_LIBS = OFF")
set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build shared libraries")

# Variables
# Variables  per configuration


# Preprocessor definitions
# Preprocessor definitions per configuration


if(CMAKE_POLICY_DEFAULT_CMP0091)  # Avoid unused and not-initialized warnings
endif()