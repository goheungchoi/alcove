# texture_compressor_lib
add_library(texture_compressor_lib texture_compressor.cpp)
target_compile_features(texture_compressor_lib PRIVATE cxx_std_20)
target_include_directories(texture_compressor_lib
  PRIVATE
  ${Alcove_INCLUDE_DIR}
  ${NVTT_INCLUDE_DIRS}
  ${stb_INCLUDE_DIR}
)
target_link_libraries(texture_compressor_lib
  PRIVATE
  general NVTT_LIBRARIES
  general KTX::ktx
)

