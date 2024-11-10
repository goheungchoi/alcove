# resource_cache_lib
add_library(resource_cache_lib STATIC resource_manager.cpp)
target_compile_features(resource_cache_lib PRIVATE cxx_std_20)
target_include_directories(resource_cache_lib PRIVATE ${Alcove_INCLUDE_DIR})
# target_link_libraries(resource_cache_lib PRIVATE resource_lib)

# resource_manager_lib
add_library(resource_manager_lib STATIC resource_manager.cpp)
target_compile_features(resource_manager_lib PRIVATE cxx_std_20)
target_include_directories(resource_manager_lib PRIVATE ${Alcove_INCLUDE_DIR})
target_link_libraries(resource_manager_lib PRIVATE resource_cache_lib)

##########################################
###### Add Subdirectories ################
##########################################


##########################################
##########################################