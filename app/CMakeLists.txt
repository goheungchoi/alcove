# GENERATE THE MAIN EXECUTABLE TARGET
add_executable(${PROJECT_NAME} main.cpp)
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)

# INCLUDE -Iinclude
target_include_directories(
  ${PROJECT_NAME} 
  PUBLIC 
  ${Alcove_INCLUDE_DIR}
  ${Vulkan_INCLUDE_DIRS}
  ${vulkan-memory-allocator_INCLUDE_DIRS}
  ${fmt_INCLUDE_DIRS}
  ${glm_INCLUDE_DIRS}
  ${SDL2_INCLUDE_DIRS}
)
# LINK  -lvk_engine_lib
target_link_libraries(
  ${PROJECT_NAME} 
  PUBLIC
  vk_engine_lib
)