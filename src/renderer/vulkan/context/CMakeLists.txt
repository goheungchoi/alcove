set(VK_COMMON_INCLUDE_DIRS 
  ${Alcove_INCLUDE_DIR}
  ${Vulkan_INCLUDE_DIRS}
  ${vulkan-memory-allocator_INCLUDE_DIRS}
  ${fmt_INCLUDE_DIRS}
  ${glm_INCLUDE_DIRS}
)

set(VK_COMMON_LIBS
  general Vulkan::Vulkan  # general means debug & release mode
  general fmt::fmt
  general glm::glm
)

# Add the 'vk_context' library target
add_library(vk_context_lib STATIC vk_context.cpp)
target_compile_features(vk_context_lib PRIVATE cxx_std_20)
target_include_directories(
  vk_context_lib
  PRIVATE
  ${VK_COMMON_INCLUDE_DIRS}
)
target_link_libraries(
  vk_context_lib 
  PRIVATE 
  ${VK_COMMON_LIBS}
)

# Add the 'vk_render_device' library target
add_library(vk_render_device_lib STATIC vk_render_device.cpp)
target_compile_features(vk_render_device_lib PRIVATE cxx_std_20)
target_include_directories(
  vk_render_device_lib
  PUBLIC
  ${VK_COMMON_INCLUDE_DIRS}
  ${SDL2_INCLUDE_DIRS}
)
target_link_libraries(
  vk_render_device_lib 
  PRIVATE 
  ${VK_COMMON_LIBS}
  general SDL2::SDL2
  general SDL2::SDL2main 
  general vk_instance_lib
  general vk_swapchain_lib
)

##########################################
###### Add Subdirectories ################
##########################################
add_subdirectory(vk-components)
add_subdirectory(vk-resources)

##########################################
##########################################