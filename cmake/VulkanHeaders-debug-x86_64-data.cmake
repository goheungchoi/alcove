########### AGGREGATED COMPONENTS AND DEPENDENCIES FOR THE MULTI CONFIG #####################
#############################################################################################

list(APPEND vulkan-headers_COMPONENT_NAMES Vulkan::Headers Vulkan::Registry)
list(REMOVE_DUPLICATES vulkan-headers_COMPONENT_NAMES)
set(vulkan-headers_FIND_DEPENDENCY_NAMES "")

########### VARIABLES #######################################################################
#############################################################################################
set(vulkan-headers_PACKAGE_FOLDER_DEBUG "C:/Users/User/.conan2/p/vulkaa955012f78a68/p")
set(vulkan-headers_BUILD_MODULES_PATHS_DEBUG )


set(vulkan-headers_INCLUDE_DIRS_DEBUG "${vulkan-headers_PACKAGE_FOLDER_DEBUG}/res/vulkan/registry"
			"${vulkan-headers_PACKAGE_FOLDER_DEBUG}/include")
set(vulkan-headers_RES_DIRS_DEBUG "${vulkan-headers_PACKAGE_FOLDER_DEBUG}/res")
set(vulkan-headers_DEFINITIONS_DEBUG )
set(vulkan-headers_SHARED_LINK_FLAGS_DEBUG )
set(vulkan-headers_EXE_LINK_FLAGS_DEBUG )
set(vulkan-headers_OBJECTS_DEBUG )
set(vulkan-headers_COMPILE_DEFINITIONS_DEBUG )
set(vulkan-headers_COMPILE_OPTIONS_C_DEBUG )
set(vulkan-headers_COMPILE_OPTIONS_CXX_DEBUG )
set(vulkan-headers_LIB_DIRS_DEBUG )
set(vulkan-headers_BIN_DIRS_DEBUG )
set(vulkan-headers_LIBRARY_TYPE_DEBUG UNKNOWN)
set(vulkan-headers_IS_HOST_WINDOWS_DEBUG 1)
set(vulkan-headers_LIBS_DEBUG )
set(vulkan-headers_SYSTEM_LIBS_DEBUG )
set(vulkan-headers_FRAMEWORK_DIRS_DEBUG )
set(vulkan-headers_FRAMEWORKS_DEBUG )
set(vulkan-headers_BUILD_DIRS_DEBUG )
set(vulkan-headers_NO_SONAME_MODE_DEBUG FALSE)


# COMPOUND VARIABLES
set(vulkan-headers_COMPILE_OPTIONS_DEBUG
    "$<$<COMPILE_LANGUAGE:CXX>:${vulkan-headers_COMPILE_OPTIONS_CXX_DEBUG}>"
    "$<$<COMPILE_LANGUAGE:C>:${vulkan-headers_COMPILE_OPTIONS_C_DEBUG}>")
set(vulkan-headers_LINKER_FLAGS_DEBUG
    "$<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,SHARED_LIBRARY>:${vulkan-headers_SHARED_LINK_FLAGS_DEBUG}>"
    "$<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,MODULE_LIBRARY>:${vulkan-headers_SHARED_LINK_FLAGS_DEBUG}>"
    "$<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,EXECUTABLE>:${vulkan-headers_EXE_LINK_FLAGS_DEBUG}>")


set(vulkan-headers_COMPONENTS_DEBUG Vulkan::Headers Vulkan::Registry)
########### COMPONENT Vulkan::Registry VARIABLES ############################################

set(vulkan-headers_Vulkan_Registry_INCLUDE_DIRS_DEBUG "${vulkan-headers_PACKAGE_FOLDER_DEBUG}/res/vulkan/registry")
set(vulkan-headers_Vulkan_Registry_LIB_DIRS_DEBUG )
set(vulkan-headers_Vulkan_Registry_BIN_DIRS_DEBUG )
set(vulkan-headers_Vulkan_Registry_LIBRARY_TYPE_DEBUG UNKNOWN)
set(vulkan-headers_Vulkan_Registry_IS_HOST_WINDOWS_DEBUG 1)
set(vulkan-headers_Vulkan_Registry_RES_DIRS_DEBUG "${vulkan-headers_PACKAGE_FOLDER_DEBUG}/res")
set(vulkan-headers_Vulkan_Registry_DEFINITIONS_DEBUG )
set(vulkan-headers_Vulkan_Registry_OBJECTS_DEBUG )
set(vulkan-headers_Vulkan_Registry_COMPILE_DEFINITIONS_DEBUG )
set(vulkan-headers_Vulkan_Registry_COMPILE_OPTIONS_C_DEBUG "")
set(vulkan-headers_Vulkan_Registry_COMPILE_OPTIONS_CXX_DEBUG "")
set(vulkan-headers_Vulkan_Registry_LIBS_DEBUG )
set(vulkan-headers_Vulkan_Registry_SYSTEM_LIBS_DEBUG )
set(vulkan-headers_Vulkan_Registry_FRAMEWORK_DIRS_DEBUG )
set(vulkan-headers_Vulkan_Registry_FRAMEWORKS_DEBUG )
set(vulkan-headers_Vulkan_Registry_DEPENDENCIES_DEBUG )
set(vulkan-headers_Vulkan_Registry_SHARED_LINK_FLAGS_DEBUG )
set(vulkan-headers_Vulkan_Registry_EXE_LINK_FLAGS_DEBUG )
set(vulkan-headers_Vulkan_Registry_NO_SONAME_MODE_DEBUG FALSE)

# COMPOUND VARIABLES
set(vulkan-headers_Vulkan_Registry_LINKER_FLAGS_DEBUG
        $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,SHARED_LIBRARY>:${vulkan-headers_Vulkan_Registry_SHARED_LINK_FLAGS_DEBUG}>
        $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,MODULE_LIBRARY>:${vulkan-headers_Vulkan_Registry_SHARED_LINK_FLAGS_DEBUG}>
        $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,EXECUTABLE>:${vulkan-headers_Vulkan_Registry_EXE_LINK_FLAGS_DEBUG}>
)
set(vulkan-headers_Vulkan_Registry_COMPILE_OPTIONS_DEBUG
    "$<$<COMPILE_LANGUAGE:CXX>:${vulkan-headers_Vulkan_Registry_COMPILE_OPTIONS_CXX_DEBUG}>"
    "$<$<COMPILE_LANGUAGE:C>:${vulkan-headers_Vulkan_Registry_COMPILE_OPTIONS_C_DEBUG}>")
########### COMPONENT Vulkan::Headers VARIABLES ############################################

set(vulkan-headers_Vulkan_Headers_INCLUDE_DIRS_DEBUG "${vulkan-headers_PACKAGE_FOLDER_DEBUG}/include")
set(vulkan-headers_Vulkan_Headers_LIB_DIRS_DEBUG )
set(vulkan-headers_Vulkan_Headers_BIN_DIRS_DEBUG )
set(vulkan-headers_Vulkan_Headers_LIBRARY_TYPE_DEBUG UNKNOWN)
set(vulkan-headers_Vulkan_Headers_IS_HOST_WINDOWS_DEBUG 1)
set(vulkan-headers_Vulkan_Headers_RES_DIRS_DEBUG )
set(vulkan-headers_Vulkan_Headers_DEFINITIONS_DEBUG )
set(vulkan-headers_Vulkan_Headers_OBJECTS_DEBUG )
set(vulkan-headers_Vulkan_Headers_COMPILE_DEFINITIONS_DEBUG )
set(vulkan-headers_Vulkan_Headers_COMPILE_OPTIONS_C_DEBUG "")
set(vulkan-headers_Vulkan_Headers_COMPILE_OPTIONS_CXX_DEBUG "")
set(vulkan-headers_Vulkan_Headers_LIBS_DEBUG )
set(vulkan-headers_Vulkan_Headers_SYSTEM_LIBS_DEBUG )
set(vulkan-headers_Vulkan_Headers_FRAMEWORK_DIRS_DEBUG )
set(vulkan-headers_Vulkan_Headers_FRAMEWORKS_DEBUG )
set(vulkan-headers_Vulkan_Headers_DEPENDENCIES_DEBUG )
set(vulkan-headers_Vulkan_Headers_SHARED_LINK_FLAGS_DEBUG )
set(vulkan-headers_Vulkan_Headers_EXE_LINK_FLAGS_DEBUG )
set(vulkan-headers_Vulkan_Headers_NO_SONAME_MODE_DEBUG FALSE)

# COMPOUND VARIABLES
set(vulkan-headers_Vulkan_Headers_LINKER_FLAGS_DEBUG
        $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,SHARED_LIBRARY>:${vulkan-headers_Vulkan_Headers_SHARED_LINK_FLAGS_DEBUG}>
        $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,MODULE_LIBRARY>:${vulkan-headers_Vulkan_Headers_SHARED_LINK_FLAGS_DEBUG}>
        $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,EXECUTABLE>:${vulkan-headers_Vulkan_Headers_EXE_LINK_FLAGS_DEBUG}>
)
set(vulkan-headers_Vulkan_Headers_COMPILE_OPTIONS_DEBUG
    "$<$<COMPILE_LANGUAGE:CXX>:${vulkan-headers_Vulkan_Headers_COMPILE_OPTIONS_CXX_DEBUG}>"
    "$<$<COMPILE_LANGUAGE:C>:${vulkan-headers_Vulkan_Headers_COMPILE_OPTIONS_C_DEBUG}>")