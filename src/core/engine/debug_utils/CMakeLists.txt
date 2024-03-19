# Add the `vk_debug_utils` library target
add_library(vk_debug_utils_lib STATIC vk_debug_utils.cpp)

target_compile_features(vk_debug_utils_lib PRIVATE cxx_std_20)
target_include_directories(
  vk_debug_utils_lib
  PRIVATE
  ${Alcove_INCLUDE_DIR}
  ${Vulkan_INCLUDE_DIRS}
  ${vulkan-memory-allocator_INCLUDE_DIRS}
  ${fmt_INCLUDE_DIRS}
  ${glm_INCLUDE_DIRS}
)
target_link_libraries(
  shader_compiler_lib 
  PRIVATE 
  Vulkan::Vulkan
  vulkan-memory-allocator::vulkan-memory-allocator
  fmt::fmt
  glm::glm
)
