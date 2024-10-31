########## MACROS ###########################################################################
#############################################################################################

# Requires CMake > 3.15
if(${CMAKE_VERSION} VERSION_LESS "3.15")
    message(FATAL_ERROR "The 'CMakeDeps' generator only works with CMake >= 3.15")
endif()

if(stduuid_FIND_QUIETLY)
    set(stduuid_MESSAGE_MODE VERBOSE)
else()
    set(stduuid_MESSAGE_MODE STATUS)
endif()

include(${CMAKE_CURRENT_LIST_DIR}/cmakedeps_macros.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/stduuidTargets.cmake)
include(CMakeFindDependencyMacro)

check_build_type_defined()

foreach(_DEPENDENCY ${stduuid_FIND_DEPENDENCY_NAMES} )
    # Check that we have not already called a find_package with the transitive dependency
    if(NOT ${_DEPENDENCY}_FOUND)
        find_dependency(${_DEPENDENCY} REQUIRED ${${_DEPENDENCY}_FIND_MODE})
    endif()
endforeach()

set(stduuid_VERSION_STRING "1.2.3")
set(stduuid_INCLUDE_DIRS ${stduuid_INCLUDE_DIRS_DEBUG} )
set(stduuid_INCLUDE_DIR ${stduuid_INCLUDE_DIRS_DEBUG} )
set(stduuid_LIBRARIES ${stduuid_LIBRARIES_DEBUG} )
set(stduuid_DEFINITIONS ${stduuid_DEFINITIONS_DEBUG} )

# Only the first installed configuration is included to avoid the collision
foreach(_BUILD_MODULE ${stduuid_BUILD_MODULES_PATHS_DEBUG} )
    message(${stduuid_MESSAGE_MODE} "Conan: Including build module from '${_BUILD_MODULE}'")
    include(${_BUILD_MODULE})
endforeach()


