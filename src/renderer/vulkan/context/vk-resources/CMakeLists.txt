# Add the 'vk_instance' library target
add_library(vk_instance_lib STATIC vk_instance.cpp)
target_compile_features(vk_instance_lib PRIVATE cxx_std_20)
target_include_directories(
  vk_instance_lib
  PUBLIC
  ${VK_COMMON_INCLUDE_DIRS}
  ${SDL2_INCLUDE_DIRS}
)
target_link_libraries(
  vk_instance_lib 
  PRIVATE 
  ${VK_COMMON_LIBS}
  general SDL2::SDL2
  general SDL2::SDL2main 
)

# Add the 'vk_swapchain' library target
add_library(vk_swapchain_lib STATIC vk_swapchain.cpp)
target_compile_features(vk_swapchain_lib PRIVATE cxx_std_20)
target_include_directories(
  vk_swapchain_lib
  PUBLIC
  ${VK_COMMON_INCLUDE_DIRS}
)
target_link_libraries(
  vk_swapchain_lib 
  PRIVATE 
  ${VK_COMMON_LIBS}
)
