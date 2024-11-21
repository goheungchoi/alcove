# importer_lib
add_library(importer_lib STATIC importer.cpp)
target_compile_features(importer_lib PRIVATE cxx_std_20)
target_include_directories(importer_lib PRIVATE ${Alcove_INCLUDE_DIR})
target_link_libraries(importer_lib PUBLIC 
  general uuid_lib
  general nlohmann_json::nlohmann_json  
)

# shader_importer_lib
add_library(shader_importer_lib STATIC shader_importer.cpp)
target_compile_features(shader_importer_lib PRIVATE cxx_std_20)
target_include_directories(shader_importer_lib PRIVATE ${Alcove_INCLUDE_DIR})
target_link_libraries(shader_importer_lib 
  PRIVATE 
  general importer_lib
)

# texture_importer_lib
add_library(texture_importer_lib STATIC texture_importer.cpp)
target_compile_features(texture_importer_lib PRIVATE cxx_std_20)
target_include_directories(texture_importer_lib PRIVATE ${Alcove_INCLUDE_DIR})
target_link_libraries(texture_importer_lib 
  PRIVATE 
  general importer_lib
  general texture_compressor_lib
)

