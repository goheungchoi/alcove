# resource_lib
add_library(resource_lib STATIC resource.cpp)
target_compile_features(resource_lib PRIVATE cxx_std_20)
target_include_directories(resource_lib PRIVATE ${Alcove_INCLUDE_DIR})
target_link_libraries(resource_lib PUBLIC general uuid_lib)

# texture_resource_lib
add_library(texture_resource_lib STATIC texture_resource.cpp)
target_compile_features(texture_resource_lib PRIVATE cxx_std_20)
target_include_directories(texture_resource_lib PRIVATE ${Alcove_INCLUDE_DIR})
target_link_libraries(texture_resource_lib PRIVATE general resource_lib)


