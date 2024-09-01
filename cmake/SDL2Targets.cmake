# Load the debug and release variables
file(GLOB DATA_FILES "${CMAKE_CURRENT_LIST_DIR}/SDL2-*-data.cmake")

foreach(f ${DATA_FILES})
    include(${f})
endforeach()

# Create the targets for all the components
foreach(_COMPONENT ${sdl_COMPONENT_NAMES} )
    if(NOT TARGET ${_COMPONENT})
        add_library(${_COMPONENT} INTERFACE IMPORTED)
        message(${SDL2_MESSAGE_MODE} "Conan: Component target declared '${_COMPONENT}'")
    endif()
endforeach()

if(NOT TARGET SDL2::SDL2main)
    add_library(SDL2::SDL2main INTERFACE IMPORTED)
    message(${SDL2_MESSAGE_MODE} "Conan: Target declared 'SDL2::SDL2main'")
endif()
if(NOT TARGET SDL2::SDL2-static)
    add_library(SDL2::SDL2-static INTERFACE IMPORTED)
    set_property(TARGET SDL2::SDL2-static PROPERTY INTERFACE_LINK_LIBRARIES SDL2::SDL2)
endif()
# Load the debug and release library finders
file(GLOB CONFIG_FILES "${CMAKE_CURRENT_LIST_DIR}/SDL2-Target-*.cmake")

foreach(f ${CONFIG_FILES})
    include(${f})
endforeach()