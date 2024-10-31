# Avoid multiple calls to find_package to append duplicated properties to the targets
include_guard()########### VARIABLES #######################################################################
#############################################################################################
set(ktx_FRAMEWORKS_FOUND_DEBUG "") # Will be filled later
conan_find_apple_frameworks(ktx_FRAMEWORKS_FOUND_DEBUG "${ktx_FRAMEWORKS_DEBUG}" "${ktx_FRAMEWORK_DIRS_DEBUG}")

set(ktx_LIBRARIES_TARGETS "") # Will be filled later


######## Create an interface target to contain all the dependencies (frameworks, system and conan deps)
if(NOT TARGET ktx_DEPS_TARGET)
    add_library(ktx_DEPS_TARGET INTERFACE IMPORTED)
endif()

set_property(TARGET ktx_DEPS_TARGET
             PROPERTY INTERFACE_LINK_LIBRARIES
             $<$<CONFIG:Debug>:${ktx_FRAMEWORKS_FOUND_DEBUG}>
             $<$<CONFIG:Debug>:${ktx_SYSTEM_LIBS_DEBUG}>
             $<$<CONFIG:Debug>:zstd::libzstd_static>
             APPEND)

####### Find the libraries declared in cpp_info.libs, create an IMPORTED target for each one and link the
####### ktx_DEPS_TARGET to all of them
conan_package_library_targets("${ktx_LIBS_DEBUG}"    # libraries
                              "${ktx_LIB_DIRS_DEBUG}" # package_libdir
                              "${ktx_BIN_DIRS_DEBUG}" # package_bindir
                              "${ktx_LIBRARY_TYPE_DEBUG}"
                              "${ktx_IS_HOST_WINDOWS_DEBUG}"
                              ktx_DEPS_TARGET
                              ktx_LIBRARIES_TARGETS  # out_libraries_targets
                              "_DEBUG"
                              "ktx"    # package_name
                              "${ktx_NO_SONAME_MODE_DEBUG}")  # soname

# FIXME: What is the result of this for multi-config? All configs adding themselves to path?
set(CMAKE_MODULE_PATH ${ktx_BUILD_DIRS_DEBUG} ${CMAKE_MODULE_PATH})

########## COMPONENTS TARGET PROPERTIES Debug ########################################

    ########## COMPONENT KTX::ktx #############

        set(ktx_KTX_ktx_FRAMEWORKS_FOUND_DEBUG "")
        conan_find_apple_frameworks(ktx_KTX_ktx_FRAMEWORKS_FOUND_DEBUG "${ktx_KTX_ktx_FRAMEWORKS_DEBUG}" "${ktx_KTX_ktx_FRAMEWORK_DIRS_DEBUG}")

        set(ktx_KTX_ktx_LIBRARIES_TARGETS "")

        ######## Create an interface target to contain all the dependencies (frameworks, system and conan deps)
        if(NOT TARGET ktx_KTX_ktx_DEPS_TARGET)
            add_library(ktx_KTX_ktx_DEPS_TARGET INTERFACE IMPORTED)
        endif()

        set_property(TARGET ktx_KTX_ktx_DEPS_TARGET
                     PROPERTY INTERFACE_LINK_LIBRARIES
                     $<$<CONFIG:Debug>:${ktx_KTX_ktx_FRAMEWORKS_FOUND_DEBUG}>
                     $<$<CONFIG:Debug>:${ktx_KTX_ktx_SYSTEM_LIBS_DEBUG}>
                     $<$<CONFIG:Debug>:${ktx_KTX_ktx_DEPENDENCIES_DEBUG}>
                     APPEND)

        ####### Find the libraries declared in cpp_info.component["xxx"].libs,
        ####### create an IMPORTED target for each one and link the 'ktx_KTX_ktx_DEPS_TARGET' to all of them
        conan_package_library_targets("${ktx_KTX_ktx_LIBS_DEBUG}"
                              "${ktx_KTX_ktx_LIB_DIRS_DEBUG}"
                              "${ktx_KTX_ktx_BIN_DIRS_DEBUG}" # package_bindir
                              "${ktx_KTX_ktx_LIBRARY_TYPE_DEBUG}"
                              "${ktx_KTX_ktx_IS_HOST_WINDOWS_DEBUG}"
                              ktx_KTX_ktx_DEPS_TARGET
                              ktx_KTX_ktx_LIBRARIES_TARGETS
                              "_DEBUG"
                              "ktx_KTX_ktx"
                              "${ktx_KTX_ktx_NO_SONAME_MODE_DEBUG}")


        ########## TARGET PROPERTIES #####################################
        set_property(TARGET KTX::ktx
                     PROPERTY INTERFACE_LINK_LIBRARIES
                     $<$<CONFIG:Debug>:${ktx_KTX_ktx_OBJECTS_DEBUG}>
                     $<$<CONFIG:Debug>:${ktx_KTX_ktx_LIBRARIES_TARGETS}>
                     APPEND)

        if("${ktx_KTX_ktx_LIBS_DEBUG}" STREQUAL "")
            # If the component is not declaring any "cpp_info.components['foo'].libs" the system, frameworks etc are not
            # linked to the imported targets and we need to do it to the global target
            set_property(TARGET KTX::ktx
                         PROPERTY INTERFACE_LINK_LIBRARIES
                         ktx_KTX_ktx_DEPS_TARGET
                         APPEND)
        endif()

        set_property(TARGET KTX::ktx PROPERTY INTERFACE_LINK_OPTIONS
                     $<$<CONFIG:Debug>:${ktx_KTX_ktx_LINKER_FLAGS_DEBUG}> APPEND)
        set_property(TARGET KTX::ktx PROPERTY INTERFACE_INCLUDE_DIRECTORIES
                     $<$<CONFIG:Debug>:${ktx_KTX_ktx_INCLUDE_DIRS_DEBUG}> APPEND)
        set_property(TARGET KTX::ktx PROPERTY INTERFACE_LINK_DIRECTORIES
                     $<$<CONFIG:Debug>:${ktx_KTX_ktx_LIB_DIRS_DEBUG}> APPEND)
        set_property(TARGET KTX::ktx PROPERTY INTERFACE_COMPILE_DEFINITIONS
                     $<$<CONFIG:Debug>:${ktx_KTX_ktx_COMPILE_DEFINITIONS_DEBUG}> APPEND)
        set_property(TARGET KTX::ktx PROPERTY INTERFACE_COMPILE_OPTIONS
                     $<$<CONFIG:Debug>:${ktx_KTX_ktx_COMPILE_OPTIONS_DEBUG}> APPEND)

    ########## AGGREGATED GLOBAL TARGET WITH THE COMPONENTS #####################
    set_property(TARGET KTX::ktx PROPERTY INTERFACE_LINK_LIBRARIES KTX::ktx APPEND)

########## For the modules (FindXXX)
set(ktx_LIBRARIES_DEBUG KTX::ktx)
