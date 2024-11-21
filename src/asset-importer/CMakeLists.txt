
# asset_importer_lib
add_library(asset_importer_lib STATIC asset_importer.cpp)
target_compile_features(asset_importer_lib PRIVATE cxx_std_20)
target_include_directories(asset_importer_lib PRIVATE ${Alcove_INCLUDE_DIR})
target_link_libraries(asset_importer_lib PRIVATE 
  general texture_importer_lib
)


##########################################
###### Add Subdirectories ################
##########################################
add_subdirectory(importer)
add_subdirectory(shader-compiler)
add_subdirectory(texture-compressor)

##########################################
##########################################
