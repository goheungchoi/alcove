########### AGGREGATED COMPONENTS AND DEPENDENCIES FOR THE MULTI CONFIG #####################
#############################################################################################

set(vulkan-memory-allocator_COMPONENT_NAMES "")
list(APPEND vulkan-memory-allocator_FIND_DEPENDENCY_NAMES VulkanHeaders)
list(REMOVE_DUPLICATES vulkan-memory-allocator_FIND_DEPENDENCY_NAMES)
set(VulkanHeaders_FIND_MODE "NO_MODULE")

########### VARIABLES #######################################################################
#############################################################################################
set(vulkan-memory-allocator_PACKAGE_FOLDER_DEBUG "C:/Users/User/.conan2/p/vulka1b3693806bfa4/p")
set(vulkan-memory-allocator_BUILD_MODULES_PATHS_DEBUG )


set(vulkan-memory-allocator_INCLUDE_DIRS_DEBUG "${vulkan-memory-allocator_PACKAGE_FOLDER_DEBUG}/include")
set(vulkan-memory-allocator_RES_DIRS_DEBUG )
set(vulkan-memory-allocator_DEFINITIONS_DEBUG )
set(vulkan-memory-allocator_SHARED_LINK_FLAGS_DEBUG )
set(vulkan-memory-allocator_EXE_LINK_FLAGS_DEBUG )
set(vulkan-memory-allocator_OBJECTS_DEBUG )
set(vulkan-memory-allocator_COMPILE_DEFINITIONS_DEBUG )
set(vulkan-memory-allocator_COMPILE_OPTIONS_C_DEBUG )
set(vulkan-memory-allocator_COMPILE_OPTIONS_CXX_DEBUG )
set(vulkan-memory-allocator_LIB_DIRS_DEBUG )
set(vulkan-memory-allocator_BIN_DIRS_DEBUG )
set(vulkan-memory-allocator_LIBRARY_TYPE_DEBUG UNKNOWN)
set(vulkan-memory-allocator_IS_HOST_WINDOWS_DEBUG 1)
set(vulkan-memory-allocator_LIBS_DEBUG )
set(vulkan-memory-allocator_SYSTEM_LIBS_DEBUG )
set(vulkan-memory-allocator_FRAMEWORK_DIRS_DEBUG )
set(vulkan-memory-allocator_FRAMEWORKS_DEBUG )
set(vulkan-memory-allocator_BUILD_DIRS_DEBUG )
set(vulkan-memory-allocator_NO_SONAME_MODE_DEBUG FALSE)


# COMPOUND VARIABLES
set(vulkan-memory-allocator_COMPILE_OPTIONS_DEBUG
    "$<$<COMPILE_LANGUAGE:CXX>:${vulkan-memory-allocator_COMPILE_OPTIONS_CXX_DEBUG}>"
    "$<$<COMPILE_LANGUAGE:C>:${vulkan-memory-allocator_COMPILE_OPTIONS_C_DEBUG}>")
set(vulkan-memory-allocator_LINKER_FLAGS_DEBUG
    "$<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,SHARED_LIBRARY>:${vulkan-memory-allocator_SHARED_LINK_FLAGS_DEBUG}>"
    "$<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,MODULE_LIBRARY>:${vulkan-memory-allocator_SHARED_LINK_FLAGS_DEBUG}>"
    "$<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,EXECUTABLE>:${vulkan-memory-allocator_EXE_LINK_FLAGS_DEBUG}>")


set(vulkan-memory-allocator_COMPONENTS_DEBUG )