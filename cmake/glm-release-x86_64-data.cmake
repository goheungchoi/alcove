########### AGGREGATED COMPONENTS AND DEPENDENCIES FOR THE MULTI CONFIG #####################
#############################################################################################

set(glm_COMPONENT_NAMES "")
set(glm_FIND_DEPENDENCY_NAMES "")

########### VARIABLES #######################################################################
#############################################################################################
set(glm_PACKAGE_FOLDER_RELEASE "C:/Users/ga1ox/.conan2/p/glm67cb345896424/p")
set(glm_BUILD_MODULES_PATHS_RELEASE )


set(glm_INCLUDE_DIRS_RELEASE "${glm_PACKAGE_FOLDER_RELEASE}/include")
set(glm_RES_DIRS_RELEASE )
set(glm_DEFINITIONS_RELEASE )
set(glm_SHARED_LINK_FLAGS_RELEASE )
set(glm_EXE_LINK_FLAGS_RELEASE )
set(glm_OBJECTS_RELEASE )
set(glm_COMPILE_DEFINITIONS_RELEASE )
set(glm_COMPILE_OPTIONS_C_RELEASE )
set(glm_COMPILE_OPTIONS_CXX_RELEASE )
set(glm_LIB_DIRS_RELEASE )
set(glm_BIN_DIRS_RELEASE )
set(glm_LIBRARY_TYPE_RELEASE UNKNOWN)
set(glm_IS_HOST_WINDOWS_RELEASE 1)
set(glm_LIBS_RELEASE )
set(glm_SYSTEM_LIBS_RELEASE )
set(glm_FRAMEWORK_DIRS_RELEASE )
set(glm_FRAMEWORKS_RELEASE )
set(glm_BUILD_DIRS_RELEASE )
set(glm_NO_SONAME_MODE_RELEASE FALSE)


# COMPOUND VARIABLES
set(glm_COMPILE_OPTIONS_RELEASE
    "$<$<COMPILE_LANGUAGE:CXX>:${glm_COMPILE_OPTIONS_CXX_RELEASE}>"
    "$<$<COMPILE_LANGUAGE:C>:${glm_COMPILE_OPTIONS_C_RELEASE}>")
set(glm_LINKER_FLAGS_RELEASE
    "$<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,SHARED_LIBRARY>:${glm_SHARED_LINK_FLAGS_RELEASE}>"
    "$<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,MODULE_LIBRARY>:${glm_SHARED_LINK_FLAGS_RELEASE}>"
    "$<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,EXECUTABLE>:${glm_EXE_LINK_FLAGS_RELEASE}>")


set(glm_COMPONENTS_RELEASE )