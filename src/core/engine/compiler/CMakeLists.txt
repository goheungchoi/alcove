# Add the `compiler` library target
add_library(shader_compiler_lib STATIC shader_compiler.cpp)

target_compile_features(shader_compiler_lib PRIVATE cxx_std_20)
target_include_directories(
  shader_compiler_lib 
  PRIVATE 
  ${Alcove_INCLUDE_DIR} 
  ${Vulkan_INCLUDE_DIR}
)
target_link_libraries(
  shader_compiler_lib 
  PRIVATE 
  ${Vulkan_LIBRARIES}
  Vulkan::Vulkan
  Vulkan::glslang
  Vulkan::SPIRV-Tools
  "${glslang-default-resource-limits_lib}"
  "${Vulkan_SPIRV-Tools-opt}"
)
