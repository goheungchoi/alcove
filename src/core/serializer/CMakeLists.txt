# serializer_lib
add_library(serializer_lib STATIC serializer.cpp)
target_compile_features(serializer_lib PRIVATE cxx_std_20)
target_include_directories(serializer_lib PRIVATE ${Alcove_INCLUDE_DIR})
target_link_libraries(serializer_lib PUBLIC general nlohmann_json::nlohmann_json)

# deserializer_lib
add_library(deserializer_lib STATIC deserializer.cpp)
target_compile_features(deserializer_lib PRIVATE cxx_std_20)
target_include_directories(deserializer_lib PRIVATE ${Alcove_INCLUDE_DIR})
target_link_libraries(deserializer_lib PUBLIC general nlohmann_json::nlohmann_json)
