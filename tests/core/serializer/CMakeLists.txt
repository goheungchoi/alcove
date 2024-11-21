# serializer_test
add_executable(serializer_test serializer_test.cpp)

target_compile_features(serializer_test PRIVATE cxx_std_20)
target_include_directories(
  serializer_test 
  PRIVATE 
  ${Alcove_INCLUDE_DIR} 
  ${Catch2_INCLUDE_DIRS}
)
target_link_libraries(
  serializer_test 
  PRIVATE 
  serializer_lib
  Catch2::Catch2WithMain
)

# Add test
add_test(
  NAME serializer_test
  COMMAND serializer_test
)
