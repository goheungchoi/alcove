# GENERATE THE MAIN EXECUTABLE TARGET
add_executable(${PROJECT_NAME} main.cpp)
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)

# INCLUDE -Iinclude
target_include_directories(${PROJECT_NAME} PUBLIC ${Alcove_INCLUDE_DIR})
# LINK  -llibraries -lvulcan -lglm -lsdl -lcatch2
target_link_libraries(
  ${PROJECT_NAME} 
  PUBLIC
  Vulkan::Vulkan
  SDL2::SDL2
  SDL2::SDL2main 
  glm::glm
)