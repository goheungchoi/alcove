# uuid_lib
add_library(uuid_lib STATIC uuid.cpp)
target_compile_features(uuid_lib PRIVATE cxx_std_20)
target_include_directories(uuid_lib PRIVATE ${Alcove_INCLUDE_DIR})
target_link_libraries(uuid_lib PRIVATE general stduuid::stduuid)
