# Load the debug and release variables
file(GLOB DATA_FILES "${CMAKE_CURRENT_LIST_DIR}/vulkan-memory-allocator-*-data.cmake")

foreach(f ${DATA_FILES})
    include(${f})
endforeach()

# Create the targets for all the components
foreach(_COMPONENT ${vulkan-memory-allocator_COMPONENT_NAMES} )
    if(NOT TARGET ${_COMPONENT})
        add_library(${_COMPONENT} INTERFACE IMPORTED)
        message(${vulkan-memory-allocator_MESSAGE_MODE} "Conan: Component target declared '${_COMPONENT}'")
    endif()
endforeach()

if(NOT TARGET vulkan-memory-allocator::vulkan-memory-allocator)
    add_library(vulkan-memory-allocator::vulkan-memory-allocator INTERFACE IMPORTED)
    message(${vulkan-memory-allocator_MESSAGE_MODE} "Conan: Target declared 'vulkan-memory-allocator::vulkan-memory-allocator'")
endif()
# Load the debug and release library finders
file(GLOB CONFIG_FILES "${CMAKE_CURRENT_LIST_DIR}/vulkan-memory-allocator-Target-*.cmake")

foreach(f ${CONFIG_FILES})
    include(${f})
endforeach()