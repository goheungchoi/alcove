########### AGGREGATED COMPONENTS AND DEPENDENCIES FOR THE MULTI CONFIG #####################
#############################################################################################

list(APPEND sdl_COMPONENT_NAMES SDL2::SDL2 SDL2::SDL2main)
list(REMOVE_DUPLICATES sdl_COMPONENT_NAMES)
set(sdl_FIND_DEPENDENCY_NAMES "")

########### VARIABLES #######################################################################
#############################################################################################
set(sdl_PACKAGE_FOLDER_DEBUG "C:/Users/ga1ox/.conan2/p/b/sdl78d843de900ef/p")
set(sdl_BUILD_MODULES_PATHS_DEBUG )


set(sdl_INCLUDE_DIRS_DEBUG "${sdl_PACKAGE_FOLDER_DEBUG}/include"
			"${sdl_PACKAGE_FOLDER_DEBUG}/include/SDL2")
set(sdl_RES_DIRS_DEBUG )
set(sdl_DEFINITIONS_DEBUG )
set(sdl_SHARED_LINK_FLAGS_DEBUG )
set(sdl_EXE_LINK_FLAGS_DEBUG )
set(sdl_OBJECTS_DEBUG )
set(sdl_COMPILE_DEFINITIONS_DEBUG )
set(sdl_COMPILE_OPTIONS_C_DEBUG )
set(sdl_COMPILE_OPTIONS_CXX_DEBUG )
set(sdl_LIB_DIRS_DEBUG "${sdl_PACKAGE_FOLDER_DEBUG}/lib")
set(sdl_BIN_DIRS_DEBUG )
set(sdl_LIBRARY_TYPE_DEBUG STATIC)
set(sdl_IS_HOST_WINDOWS_DEBUG 1)
set(sdl_LIBS_DEBUG SDL2maind SDL2-staticd)
set(sdl_SYSTEM_LIBS_DEBUG user32 gdi32 winmm imm32 ole32 oleaut32 version uuid advapi32 setupapi shell32)
set(sdl_FRAMEWORK_DIRS_DEBUG )
set(sdl_FRAMEWORKS_DEBUG )
set(sdl_BUILD_DIRS_DEBUG )
set(sdl_NO_SONAME_MODE_DEBUG FALSE)


# COMPOUND VARIABLES
set(sdl_COMPILE_OPTIONS_DEBUG
    "$<$<COMPILE_LANGUAGE:CXX>:${sdl_COMPILE_OPTIONS_CXX_DEBUG}>"
    "$<$<COMPILE_LANGUAGE:C>:${sdl_COMPILE_OPTIONS_C_DEBUG}>")
set(sdl_LINKER_FLAGS_DEBUG
    "$<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,SHARED_LIBRARY>:${sdl_SHARED_LINK_FLAGS_DEBUG}>"
    "$<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,MODULE_LIBRARY>:${sdl_SHARED_LINK_FLAGS_DEBUG}>"
    "$<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,EXECUTABLE>:${sdl_EXE_LINK_FLAGS_DEBUG}>")


set(sdl_COMPONENTS_DEBUG SDL2::SDL2 SDL2::SDL2main)
########### COMPONENT SDL2::SDL2main VARIABLES ############################################

set(sdl_SDL2_SDL2main_INCLUDE_DIRS_DEBUG "${sdl_PACKAGE_FOLDER_DEBUG}/include")
set(sdl_SDL2_SDL2main_LIB_DIRS_DEBUG "${sdl_PACKAGE_FOLDER_DEBUG}/lib")
set(sdl_SDL2_SDL2main_BIN_DIRS_DEBUG )
set(sdl_SDL2_SDL2main_LIBRARY_TYPE_DEBUG STATIC)
set(sdl_SDL2_SDL2main_IS_HOST_WINDOWS_DEBUG 1)
set(sdl_SDL2_SDL2main_RES_DIRS_DEBUG )
set(sdl_SDL2_SDL2main_DEFINITIONS_DEBUG )
set(sdl_SDL2_SDL2main_OBJECTS_DEBUG )
set(sdl_SDL2_SDL2main_COMPILE_DEFINITIONS_DEBUG )
set(sdl_SDL2_SDL2main_COMPILE_OPTIONS_C_DEBUG "")
set(sdl_SDL2_SDL2main_COMPILE_OPTIONS_CXX_DEBUG "")
set(sdl_SDL2_SDL2main_LIBS_DEBUG SDL2maind)
set(sdl_SDL2_SDL2main_SYSTEM_LIBS_DEBUG )
set(sdl_SDL2_SDL2main_FRAMEWORK_DIRS_DEBUG )
set(sdl_SDL2_SDL2main_FRAMEWORKS_DEBUG )
set(sdl_SDL2_SDL2main_DEPENDENCIES_DEBUG SDL2::SDL2)
set(sdl_SDL2_SDL2main_SHARED_LINK_FLAGS_DEBUG )
set(sdl_SDL2_SDL2main_EXE_LINK_FLAGS_DEBUG )
set(sdl_SDL2_SDL2main_NO_SONAME_MODE_DEBUG FALSE)

# COMPOUND VARIABLES
set(sdl_SDL2_SDL2main_LINKER_FLAGS_DEBUG
        $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,SHARED_LIBRARY>:${sdl_SDL2_SDL2main_SHARED_LINK_FLAGS_DEBUG}>
        $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,MODULE_LIBRARY>:${sdl_SDL2_SDL2main_SHARED_LINK_FLAGS_DEBUG}>
        $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,EXECUTABLE>:${sdl_SDL2_SDL2main_EXE_LINK_FLAGS_DEBUG}>
)
set(sdl_SDL2_SDL2main_COMPILE_OPTIONS_DEBUG
    "$<$<COMPILE_LANGUAGE:CXX>:${sdl_SDL2_SDL2main_COMPILE_OPTIONS_CXX_DEBUG}>"
    "$<$<COMPILE_LANGUAGE:C>:${sdl_SDL2_SDL2main_COMPILE_OPTIONS_C_DEBUG}>")
########### COMPONENT SDL2::SDL2 VARIABLES ############################################

set(sdl_SDL2_SDL2_INCLUDE_DIRS_DEBUG "${sdl_PACKAGE_FOLDER_DEBUG}/include"
			"${sdl_PACKAGE_FOLDER_DEBUG}/include/SDL2")
set(sdl_SDL2_SDL2_LIB_DIRS_DEBUG "${sdl_PACKAGE_FOLDER_DEBUG}/lib")
set(sdl_SDL2_SDL2_BIN_DIRS_DEBUG )
set(sdl_SDL2_SDL2_LIBRARY_TYPE_DEBUG STATIC)
set(sdl_SDL2_SDL2_IS_HOST_WINDOWS_DEBUG 1)
set(sdl_SDL2_SDL2_RES_DIRS_DEBUG )
set(sdl_SDL2_SDL2_DEFINITIONS_DEBUG )
set(sdl_SDL2_SDL2_OBJECTS_DEBUG )
set(sdl_SDL2_SDL2_COMPILE_DEFINITIONS_DEBUG )
set(sdl_SDL2_SDL2_COMPILE_OPTIONS_C_DEBUG "")
set(sdl_SDL2_SDL2_COMPILE_OPTIONS_CXX_DEBUG "")
set(sdl_SDL2_SDL2_LIBS_DEBUG SDL2-staticd)
set(sdl_SDL2_SDL2_SYSTEM_LIBS_DEBUG user32 gdi32 winmm imm32 ole32 oleaut32 version uuid advapi32 setupapi shell32)
set(sdl_SDL2_SDL2_FRAMEWORK_DIRS_DEBUG )
set(sdl_SDL2_SDL2_FRAMEWORKS_DEBUG )
set(sdl_SDL2_SDL2_DEPENDENCIES_DEBUG )
set(sdl_SDL2_SDL2_SHARED_LINK_FLAGS_DEBUG )
set(sdl_SDL2_SDL2_EXE_LINK_FLAGS_DEBUG )
set(sdl_SDL2_SDL2_NO_SONAME_MODE_DEBUG FALSE)

# COMPOUND VARIABLES
set(sdl_SDL2_SDL2_LINKER_FLAGS_DEBUG
        $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,SHARED_LIBRARY>:${sdl_SDL2_SDL2_SHARED_LINK_FLAGS_DEBUG}>
        $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,MODULE_LIBRARY>:${sdl_SDL2_SDL2_SHARED_LINK_FLAGS_DEBUG}>
        $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,EXECUTABLE>:${sdl_SDL2_SDL2_EXE_LINK_FLAGS_DEBUG}>
)
set(sdl_SDL2_SDL2_COMPILE_OPTIONS_DEBUG
    "$<$<COMPILE_LANGUAGE:CXX>:${sdl_SDL2_SDL2_COMPILE_OPTIONS_CXX_DEBUG}>"
    "$<$<COMPILE_LANGUAGE:C>:${sdl_SDL2_SDL2_COMPILE_OPTIONS_C_DEBUG}>")