# Avoid multiple calls to find_package to append duplicated properties to the targets
include_guard()########### VARIABLES #######################################################################
#############################################################################################
set(vulkan-memory-allocator_FRAMEWORKS_FOUND_DEBUG "") # Will be filled later
conan_find_apple_frameworks(vulkan-memory-allocator_FRAMEWORKS_FOUND_DEBUG "${vulkan-memory-allocator_FRAMEWORKS_DEBUG}" "${vulkan-memory-allocator_FRAMEWORK_DIRS_DEBUG}")

set(vulkan-memory-allocator_LIBRARIES_TARGETS "") # Will be filled later


######## Create an interface target to contain all the dependencies (frameworks, system and conan deps)
if(NOT TARGET vulkan-memory-allocator_DEPS_TARGET)
    add_library(vulkan-memory-allocator_DEPS_TARGET INTERFACE IMPORTED)
endif()

set_property(TARGET vulkan-memory-allocator_DEPS_TARGET
             PROPERTY INTERFACE_LINK_LIBRARIES
             $<$<CONFIG:Debug>:${vulkan-memory-allocator_FRAMEWORKS_FOUND_DEBUG}>
             $<$<CONFIG:Debug>:${vulkan-memory-allocator_SYSTEM_LIBS_DEBUG}>
             $<$<CONFIG:Debug>:vulkan-headers::vulkan-headers>
             APPEND)

####### Find the libraries declared in cpp_info.libs, create an IMPORTED target for each one and link the
####### vulkan-memory-allocator_DEPS_TARGET to all of them
conan_package_library_targets("${vulkan-memory-allocator_LIBS_DEBUG}"    # libraries
                              "${vulkan-memory-allocator_LIB_DIRS_DEBUG}" # package_libdir
                              "${vulkan-memory-allocator_BIN_DIRS_DEBUG}" # package_bindir
                              "${vulkan-memory-allocator_LIBRARY_TYPE_DEBUG}"
                              "${vulkan-memory-allocator_IS_HOST_WINDOWS_DEBUG}"
                              vulkan-memory-allocator_DEPS_TARGET
                              vulkan-memory-allocator_LIBRARIES_TARGETS  # out_libraries_targets
                              "_DEBUG"
                              "vulkan-memory-allocator"    # package_name
                              "${vulkan-memory-allocator_NO_SONAME_MODE_DEBUG}")  # soname

# FIXME: What is the result of this for multi-config? All configs adding themselves to path?
set(CMAKE_MODULE_PATH ${vulkan-memory-allocator_BUILD_DIRS_DEBUG} ${CMAKE_MODULE_PATH})

########## GLOBAL TARGET PROPERTIES Debug ########################################
    set_property(TARGET vulkan-memory-allocator::vulkan-memory-allocator
                 PROPERTY INTERFACE_LINK_LIBRARIES
                 $<$<CONFIG:Debug>:${vulkan-memory-allocator_OBJECTS_DEBUG}>
                 $<$<CONFIG:Debug>:${vulkan-memory-allocator_LIBRARIES_TARGETS}>
                 APPEND)

    if("${vulkan-memory-allocator_LIBS_DEBUG}" STREQUAL "")
        # If the package is not declaring any "cpp_info.libs" the package deps, system libs,
        # frameworks etc are not linked to the imported targets and we need to do it to the
        # global target
        set_property(TARGET vulkan-memory-allocator::vulkan-memory-allocator
                     PROPERTY INTERFACE_LINK_LIBRARIES
                     vulkan-memory-allocator_DEPS_TARGET
                     APPEND)
    endif()

    set_property(TARGET vulkan-memory-allocator::vulkan-memory-allocator
                 PROPERTY INTERFACE_LINK_OPTIONS
                 $<$<CONFIG:Debug>:${vulkan-memory-allocator_LINKER_FLAGS_DEBUG}> APPEND)
    set_property(TARGET vulkan-memory-allocator::vulkan-memory-allocator
                 PROPERTY INTERFACE_INCLUDE_DIRECTORIES
                 $<$<CONFIG:Debug>:${vulkan-memory-allocator_INCLUDE_DIRS_DEBUG}> APPEND)
    # Necessary to find LINK shared libraries in Linux
    set_property(TARGET vulkan-memory-allocator::vulkan-memory-allocator
                 PROPERTY INTERFACE_LINK_DIRECTORIES
                 $<$<CONFIG:Debug>:${vulkan-memory-allocator_LIB_DIRS_DEBUG}> APPEND)
    set_property(TARGET vulkan-memory-allocator::vulkan-memory-allocator
                 PROPERTY INTERFACE_COMPILE_DEFINITIONS
                 $<$<CONFIG:Debug>:${vulkan-memory-allocator_COMPILE_DEFINITIONS_DEBUG}> APPEND)
    set_property(TARGET vulkan-memory-allocator::vulkan-memory-allocator
                 PROPERTY INTERFACE_COMPILE_OPTIONS
                 $<$<CONFIG:Debug>:${vulkan-memory-allocator_COMPILE_OPTIONS_DEBUG}> APPEND)

########## For the modules (FindXXX)
set(vulkan-memory-allocator_LIBRARIES_DEBUG vulkan-memory-allocator::vulkan-memory-allocator)
