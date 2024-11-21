# shader_compiler_lib
add_library(shader_spv_compiler_lib STATIC shader_compiler.cpp)
target_compile_features(shader_spv_compiler_lib PRIVATE cxx_std_20)
target_include_directories(shader_spv_compiler_lib
  PRIVATE
  ${Alcove_INCLUDE_DIR}
)
target_link_libraries(shader_spv_compiler_lib
  PRIVATE
  general Vulkan::shaderc_combined
)
