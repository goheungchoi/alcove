add_executable(texture_importer_test texture_importer_test.cpp)

target_compile_features(texture_importer_test PRIVATE cxx_std_20)
target_include_directories(
  texture_importer_test 
  PRIVATE 
  ${Alcove_INCLUDE_DIR} 
  ${Catch2_INCLUDE_DIRS}
)
target_link_libraries(
  texture_importer_test 
  PRIVATE 
  texture_importer_lib 
  Catch2::Catch2WithMain
)

# Add test
add_test(
  NAME texture_importer_test
  COMMAND texture_importer_test
)

##########################################
###### Add Subdirectories ################
##########################################
add_subdirectory(compressor)

##########################################
##########################################
