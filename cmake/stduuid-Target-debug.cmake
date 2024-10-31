# Avoid multiple calls to find_package to append duplicated properties to the targets
include_guard()########### VARIABLES #######################################################################
#############################################################################################
set(stduuid_FRAMEWORKS_FOUND_DEBUG "") # Will be filled later
conan_find_apple_frameworks(stduuid_FRAMEWORKS_FOUND_DEBUG "${stduuid_FRAMEWORKS_DEBUG}" "${stduuid_FRAMEWORK_DIRS_DEBUG}")

set(stduuid_LIBRARIES_TARGETS "") # Will be filled later


######## Create an interface target to contain all the dependencies (frameworks, system and conan deps)
if(NOT TARGET stduuid_DEPS_TARGET)
    add_library(stduuid_DEPS_TARGET INTERFACE IMPORTED)
endif()

set_property(TARGET stduuid_DEPS_TARGET
             PROPERTY INTERFACE_LINK_LIBRARIES
             $<$<CONFIG:Debug>:${stduuid_FRAMEWORKS_FOUND_DEBUG}>
             $<$<CONFIG:Debug>:${stduuid_SYSTEM_LIBS_DEBUG}>
             $<$<CONFIG:Debug>:>
             APPEND)

####### Find the libraries declared in cpp_info.libs, create an IMPORTED target for each one and link the
####### stduuid_DEPS_TARGET to all of them
conan_package_library_targets("${stduuid_LIBS_DEBUG}"    # libraries
                              "${stduuid_LIB_DIRS_DEBUG}" # package_libdir
                              "${stduuid_BIN_DIRS_DEBUG}" # package_bindir
                              "${stduuid_LIBRARY_TYPE_DEBUG}"
                              "${stduuid_IS_HOST_WINDOWS_DEBUG}"
                              stduuid_DEPS_TARGET
                              stduuid_LIBRARIES_TARGETS  # out_libraries_targets
                              "_DEBUG"
                              "stduuid"    # package_name
                              "${stduuid_NO_SONAME_MODE_DEBUG}")  # soname

# FIXME: What is the result of this for multi-config? All configs adding themselves to path?
set(CMAKE_MODULE_PATH ${stduuid_BUILD_DIRS_DEBUG} ${CMAKE_MODULE_PATH})

########## GLOBAL TARGET PROPERTIES Debug ########################################
    set_property(TARGET stduuid::stduuid
                 PROPERTY INTERFACE_LINK_LIBRARIES
                 $<$<CONFIG:Debug>:${stduuid_OBJECTS_DEBUG}>
                 $<$<CONFIG:Debug>:${stduuid_LIBRARIES_TARGETS}>
                 APPEND)

    if("${stduuid_LIBS_DEBUG}" STREQUAL "")
        # If the package is not declaring any "cpp_info.libs" the package deps, system libs,
        # frameworks etc are not linked to the imported targets and we need to do it to the
        # global target
        set_property(TARGET stduuid::stduuid
                     PROPERTY INTERFACE_LINK_LIBRARIES
                     stduuid_DEPS_TARGET
                     APPEND)
    endif()

    set_property(TARGET stduuid::stduuid
                 PROPERTY INTERFACE_LINK_OPTIONS
                 $<$<CONFIG:Debug>:${stduuid_LINKER_FLAGS_DEBUG}> APPEND)
    set_property(TARGET stduuid::stduuid
                 PROPERTY INTERFACE_INCLUDE_DIRECTORIES
                 $<$<CONFIG:Debug>:${stduuid_INCLUDE_DIRS_DEBUG}> APPEND)
    # Necessary to find LINK shared libraries in Linux
    set_property(TARGET stduuid::stduuid
                 PROPERTY INTERFACE_LINK_DIRECTORIES
                 $<$<CONFIG:Debug>:${stduuid_LIB_DIRS_DEBUG}> APPEND)
    set_property(TARGET stduuid::stduuid
                 PROPERTY INTERFACE_COMPILE_DEFINITIONS
                 $<$<CONFIG:Debug>:${stduuid_COMPILE_DEFINITIONS_DEBUG}> APPEND)
    set_property(TARGET stduuid::stduuid
                 PROPERTY INTERFACE_COMPILE_OPTIONS
                 $<$<CONFIG:Debug>:${stduuid_COMPILE_OPTIONS_DEBUG}> APPEND)

########## For the modules (FindXXX)
set(stduuid_LIBRARIES_DEBUG stduuid::stduuid)
