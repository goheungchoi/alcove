# Add the `engine` library target
add_library(vk_engine_lib STATIC vk_engine.cpp)

target_compile_features(vk_engine_lib PRIVATE cxx_std_20)
target_include_directories(
  vk_engine_lib 
  PRIVATE 
  ${Alcove_INCLUDE_DIR}
  ${Vulkan_INCLUDE_DIRS}
)
target_link_libraries(
  vk_engine_lib 
  PRIVATE 
  Vulkan::Vulkan
)

##########################################
###### Add Subdirectories ################
##########################################
add_subdirectory(compiler)
##########################################
##########################################
