########## MACROS ###########################################################################
#############################################################################################

# Requires CMake > 3.15
if(${CMAKE_VERSION} VERSION_LESS "3.15")
    message(FATAL_ERROR "The 'CMakeDeps' generator only works with CMake >= 3.15")
endif()

if(vulkan-memory-allocator_FIND_QUIETLY)
    set(vulkan-memory-allocator_MESSAGE_MODE VERBOSE)
else()
    set(vulkan-memory-allocator_MESSAGE_MODE STATUS)
endif()

include(${CMAKE_CURRENT_LIST_DIR}/cmakedeps_macros.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/vulkan-memory-allocatorTargets.cmake)
include(CMakeFindDependencyMacro)

check_build_type_defined()

foreach(_DEPENDENCY ${vulkan-memory-allocator_FIND_DEPENDENCY_NAMES} )
    # Check that we have not already called a find_package with the transitive dependency
    if(NOT ${_DEPENDENCY}_FOUND)
        find_dependency(${_DEPENDENCY} REQUIRED ${${_DEPENDENCY}_FIND_MODE})
    endif()
endforeach()

set(vulkan-memory-allocator_VERSION_STRING "3.0.1")
set(vulkan-memory-allocator_INCLUDE_DIRS ${vulkan-memory-allocator_INCLUDE_DIRS_RELEASE} )
set(vulkan-memory-allocator_INCLUDE_DIR ${vulkan-memory-allocator_INCLUDE_DIRS_RELEASE} )
set(vulkan-memory-allocator_LIBRARIES ${vulkan-memory-allocator_LIBRARIES_RELEASE} )
set(vulkan-memory-allocator_DEFINITIONS ${vulkan-memory-allocator_DEFINITIONS_RELEASE} )

# Only the first installed configuration is included to avoid the collision
foreach(_BUILD_MODULE ${vulkan-memory-allocator_BUILD_MODULES_PATHS_RELEASE} )
    message(${vulkan-memory-allocator_MESSAGE_MODE} "Conan: Including build module from '${_BUILD_MODULE}'")
    include(${_BUILD_MODULE})
endforeach()


