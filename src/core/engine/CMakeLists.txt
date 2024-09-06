# vk_engine common external libraries


# Add the `descriptors` library target
add_library(vk_descriptors_lib STATIC vk_descriptors.cpp)

target_compile_features(vk_descriptors_lib PRIVATE cxx_std_20)
target_include_directories(
  vk_descriptors_lib
  PRIVATE
  ${Alcove_INCLUDE_DIR}
  ${Vulkan_INCLUDE_DIRS}
  ${vulkan-memory-allocator_INCLUDE_DIRS}
  ${fmt_INCLUDE_DIRS}
  ${glm_INCLUDE_DIRS}
)
target_link_libraries(
  vk_descriptors_lib 
  PRIVATE 
  general Vulkan::Vulkan  # general means debug & release mode
)

# Add the `pipelines` library target
add_library(vk_pipelines_lib STATIC vk_pipelines.cpp)

target_compile_features(vk_pipelines_lib PRIVATE cxx_std_20)
target_include_directories(
  vk_pipelines_lib
  PRIVATE
  ${Alcove_INCLUDE_DIR}
  ${Vulkan_INCLUDE_DIRS}
  ${vulkan-memory-allocator_INCLUDE_DIRS}
  ${fmt_INCLUDE_DIRS}
  ${glm_INCLUDE_DIRS}
)
target_link_libraries(
  vk_pipelines_lib 
  PRIVATE 
  general Vulkan::Vulkan  # general means debug & release mode
)

# Add the `engine` library target
add_library(
  vk_engine_lib 
  STATIC 
  vk_engine.cpp
  
  # Bind Dear-Imgui backends
  "${imgui_BACKEND_DIR}/imgui_impl_sdl2.cpp"
  "${imgui_BACKEND_DIR}/imgui_impl_vulkan.cpp"
)

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
  ${imgui_INCLUDE_DIRS}
)
target_link_libraries(
  vk_engine_lib 
  PRIVATE 
  general Vulkan::Vulkan
  general fmt::fmt
  general glm::glm
  general SDL2::SDL2
  general SDL2::SDL2main 
  general imgui::imgui
  general vk_descriptors_lib
  general vk_pipelines_lib
)

##########################################
###### Add Subdirectories ################
##########################################
add_subdirectory(compiler)
##########################################
##########################################
