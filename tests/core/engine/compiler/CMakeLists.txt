add_executable(shader_compiler_test shader_compiler_test.cpp)

target_compile_features(shader_compiler_test PRIVATE cxx_std_20)
target_include_directories(
  shader_compiler_test 
  PRIVATE 
  ${Alcove_INCLUDE_DIR} 
  ${Catch2_INCLUDE_DIRS}
)
target_link_libraries(
  shader_compiler_test 
  PRIVATE 
  shader_compiler_lib 
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