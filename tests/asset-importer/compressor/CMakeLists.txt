add_executable(texture_compressor_test texture_compressor_test.cpp)

target_compile_features(texture_compressor_test PRIVATE cxx_std_20)
target_include_directories(
  texture_compressor_test 
  PRIVATE 
  ${Alcove_INCLUDE_DIR} 
  ${Catch2_INCLUDE_DIRS}
)
target_link_libraries(
  texture_compressor_test 
  PRIVATE 
  uuid_lib
  texture_compressor_lib 
  Catch2::Catch2WithMain
)

# Add test
add_test(
  NAME texture_compressor_test
  COMMAND texture_compressor_test
)

