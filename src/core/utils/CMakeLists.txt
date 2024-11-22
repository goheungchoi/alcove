# io_util_lib
add_library(io_util_lib STATIC io_util.cpp)

target_compile_features(io_util_lib PRIVATE cxx_std_20)
target_include_directories(io_util_lib PRIVATE ${Alcove_INCLUDE_DIR})
# target_link_libraries(
#   io_util_lib 
#   PRIVATE 
# )


