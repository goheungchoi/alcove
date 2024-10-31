# Add the 'engine' library target
add_library(engine_lib STATIC engine.cpp)
target_compile_features(engine_lib PRIVATE cxx_std_20)
target_include_directories(
  engine_lib
  PRIVATE
  ${Alcove_INCLUDE_DIR}
  ${Vulkan_INCLUDE_DIRS}
  ${fmt_INCLUDE_DIRS}
  ${SDL2_INCLUDE_DIRS}
  ${imgui_INCLUDE_DIRS}
)
target_link_libraries(
  engine_lib 
  PRIVATE 
  general fmt::fmt
  general SDL2::SDL2
  general SDL2::SDL2main 
  general imgui::imgui
  general vk_renderer_lib
)
