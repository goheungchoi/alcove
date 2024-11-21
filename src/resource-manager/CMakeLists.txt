# resource_pool_lib
add_library(resource_pool_lib STATIC resource_pool.cpp)
target_compile_features(resource_pool_lib PRIVATE cxx_std_20)
target_include_directories(resource_pool_lib 
  PRIVATE 
  ${Alcove_INCLUDE_DIR}
  ${ktx_INCLUDE_DIR}
)
target_link_libraries(resource_pool_lib 
  PUBLIC 
  general resource_lib 
  general texture_lib 
  general uuid_lib
  general Vulkan::Vulkan
  general KTX::ktx
)

# resource_manager_lib
add_library(resource_manager_lib STATIC resource_manager.cpp)
target_compile_features(resource_manager_lib PRIVATE cxx_std_20)
target_include_directories(resource_manager_lib PRIVATE ${Alcove_INCLUDE_DIR})
target_link_libraries(resource_manager_lib PRIVATE resource_pool_lib)

##########################################
###### Add Subdirectories ################
##########################################


##########################################
##########################################