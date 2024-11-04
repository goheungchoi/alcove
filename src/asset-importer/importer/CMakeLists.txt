# texture_importer_lib
add_library(texture_importer_lib STATIC texture_importer.cpp)
target_compile_features(texture_importer_lib PRIVATE cxx_std_20)
target_include_directories(texture_importer_lib PRIVATE ${Alcove_INCLUDE_DIR})
target_link_libraries(texture_importer_lib 
  PRIVATE 
  general importer_lib
  general texture_compressor_lib
)

