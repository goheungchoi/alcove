cmake_minimum_required(VERSION 3.20)

project(Alcove  VERSION     1.0.0
                LANGUAGES   C CXX
                DESCRIPTION "Vulkan Real-time Rendering Engine"
)

### Language Setting
CMAKE_CXX_STANDARD(17)
CMAKE_CXX_STANDARD_REQUIRED(17)



### Testing Option
option(BUILD_TESTING OFF)