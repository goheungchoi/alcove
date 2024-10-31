# Add the 'vk_renderer' library target
add_library(vk_renderer_lib STATIC vk_renderer.cpp)
target_compile_features(vk_renderer_lib PRIVATE cxx_std_20)

target_include_directories(
  vk_renderer_lib
  PRIVATE
  ${Alcove_INCLUDE_DIR}
  ${Vulkan_INCLUDE_DIRS}
)

# Link libraries
target_link_libraries(
  vk_renderer_lib 
  PRIVATE 
  general Vulkan::Vulkan  # general means debug & release mode
  general vk_render_device_lib
)

##########################################
###### Add Subdirectories ################
##########################################
add_subdirectory(context)

##########################################
##########################################