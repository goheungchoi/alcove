# Find the vulkan libraries and tools that are not come with
# the default VulkanFind module.
function(find_vulkan_extra)
  if (WIN32) 
    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
      set(Vulkan_BINARY_DIR "$ENV{VULKAN_SDK}/Bin")
      set(Vulkan_LIBRARY_DIR "$ENV{VULKAN_SDK}/Lib")
    else()
      set(Vulkan_BINARY_DIR "$ENV{VULKAN_SDK}/Bin32")
      set(Vulkan_LIBRARY_DIR "$ENV{VULKAN_SDK}/Lib32")
    endif()
  else()
    set(Vulkan_BINARY_DIR "$ENV{VULKAN_SDK}/bin")
    set(Vulkan_LIBRARY_DIR "$ENV{VULKAN_SDK}/lib")
  endif()

  # Vulkan glslang default resource
  find_library(
    Vulkan_glslang-default-resource-limits_LIBRARY
    NAMES glslang-default-resource-limits
    HINTS ${Vulkan_LIBRARY_DIR}
  )
  mark_as_advanced(Vulkan_glslang-default-resource-limits_LIBRARY)
  find_library(
    Vulkan_glslang-default-resource-limits_DEBUG_LIBRARY
    NAMES glslang-default-resource-limitsd
    HINTS ${Vulkan_LIBRARY_DIR}
  )
  mark_as_advanced(Vulkan_glslang-default-resource-limits_DEBUG_LIBRARY)

  set(glslang-default-resource-limits_lib 
    "$<$<CONFIG:Debug>:${Vulkan_glslang-default-resource-limits_DEBUG_LIBRARY}>$<$<NOT:$<CONFIG:Debug>>:${Vulkan_glslang-default-resource-limits_LIBRARY}>"
  )

  # Vulkan SPIRV Tools opt
  find_library(
    Vulkan_SPIRV-Tools-opt_LIBRARY
    NAMES SPIRV-Tools-opt
    HINTS ${Vulkan_LIBRARY_DIR}
  )
  mark_as_advanced(Vulkan_SPIRV-Tools-opt_LIBRARY)
  find_library(
    Vulkan_SPIRV-Tools-opt_DEBUG_LIBRARY
    NAMES SPIRV-Tools-optd
    HINTS ${Vulkan_LIBRARY_DIR}
  )
  mark_as_advanced(Vulkan_SPIRV-Tools-opt_DEBUG_LIBRARY)

  set(Vulkan_SPIRV-Tools-opt 
    "$<$<CONFIG:Debug>:${Vulkan_SPIRV-Tools-opt_DEBUG_LIBRARY}>$<$<NOT:$<CONFIG:Debug>>:${Vulkan_SPIRV-Tools-opt_LIBRARY}>"
  )

  # Vulkan Memory Allocator Header-only
  set(vulkan-memory-allocator_INCLUDE_DIRS "$ENV{VULKAN_SDK}/Include/vma")

  return(
    PROPAGATE 
    glslang-default-resource-limits_lib 
    Vulkan_SPIRV-Tools-opt 
    vulkan-memory-allocator_INCLUDE_DIRS
  )
endfunction()