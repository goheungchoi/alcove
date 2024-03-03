add_executable(shader_compiler_test shader_compiler_test.cpp)

target_compile_features(shader_compiler_test PRIVATE cxx_std_20)
target_include_directories(
  shader_compiler_test 
  PRIVATE 
  ${Alcove_INCLUDE_DIR} 
  ${Vulkan_INCLUDE_DIR}
  ${Catch2_INCLUDE_DIR}
)
target_link_libraries(
  shader_compiler_test 
  PRIVATE 
  shader_compiler_lib 
  Vulkan::Vulkan
  "C:/VulkanSDK/1.3.268.0/Lib/glslang-default-resource-limits.lib"
  Vulkan::glslang
  Vulkan::SPIRV-Tools
  "C:/VulkanSDK/1.3.268.0/Lib/SPIRV-Tools-opt.lib"
  Catch2::Catch2WithMain
)
cmake_print_variables(Vulkan_glslang_LIBRARY)
cmake_print_variables(Vulkan_SPIRV-Tools_LIBRARY)
cmake_print_variables(Vulkan_SPIRV-Tools-opt_LIBRARY)
# Add test
add_test(
  NAME shader_compiler_test
  COMMAND shader_compiler_test
)