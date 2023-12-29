# GENERATE THE MAIN EXECUTABLE TARGET
add_executable(${PROJECT_NAME} main.cpp)
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)

# INCLUDE -Iinclude
target_include_directories(${PROJECT_NAME} PUBLIC include)
# LINK  -llibraries -lvulcan -lglm
target_link_libraries(${PROJECT_NAME} libraries Vulkan::Vulkan GLM::GLM)