# Add the `engine` library target
add_library(vk_engine_lib STATIC vk_engine.cpp)

target_compile_features(vk_engine_lib PRIVATE cxx_std_20)
target_include_directories(
  vk_engine_lib 
  PRIVATE 
  ${Alcove_INCLUDE_DIR}
  ${Vulkan_INCLUDE_DIRS}
  ${vulkan-memory-allocator_INCLUDE_DIRS}
  ${fmt_INCLUDE_DIRS}
  ${glm_INCLUDE_DIRS}
  ${SDL2_INCLUDE_DIRS}
)
target_link_libraries(
  vk_engine_lib 
  PRIVATE 
  general Vulkan::Vulkan
  general vulkan-memory-allocator::vulkan-memory-allocator
  general fmt::fmt
  general glm::glm
  general SDL2::SDL2
  general SDL2::SDL2main 
)

##########################################
###### Add Subdirectories ################
##########################################
add_subdirectory(compiler)
##########################################
##########################################
