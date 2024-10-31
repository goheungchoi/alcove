########## MACROS ###########################################################################
#############################################################################################

# Requires CMake > 3.15
if(${CMAKE_VERSION} VERSION_LESS "3.15")
    message(FATAL_ERROR "The 'CMakeDeps' generator only works with CMake >= 3.15")
endif()

if(Ktx_FIND_QUIETLY)
    set(Ktx_MESSAGE_MODE VERBOSE)
else()
    set(Ktx_MESSAGE_MODE STATUS)
endif()

include(${CMAKE_CURRENT_LIST_DIR}/cmakedeps_macros.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/KtxTargets.cmake)
include(CMakeFindDependencyMacro)

check_build_type_defined()

foreach(_DEPENDENCY ${ktx_FIND_DEPENDENCY_NAMES} )
    # Check that we have not already called a find_package with the transitive dependency
    if(NOT ${_DEPENDENCY}_FOUND)
        find_dependency(${_DEPENDENCY} REQUIRED ${${_DEPENDENCY}_FIND_MODE})
    endif()
endforeach()

set(Ktx_VERSION_STRING "4.3.2")
set(Ktx_INCLUDE_DIRS ${ktx_INCLUDE_DIRS_DEBUG} )
set(Ktx_INCLUDE_DIR ${ktx_INCLUDE_DIRS_DEBUG} )
set(Ktx_LIBRARIES ${ktx_LIBRARIES_DEBUG} )
set(Ktx_DEFINITIONS ${ktx_DEFINITIONS_DEBUG} )

# Only the first installed configuration is included to avoid the collision
foreach(_BUILD_MODULE ${ktx_BUILD_MODULES_PATHS_DEBUG} )
    message(${Ktx_MESSAGE_MODE} "Conan: Including build module from '${_BUILD_MODULE}'")
    include(${_BUILD_MODULE})
endforeach()


