# GENERATE THE MAIN EXECUTABLE TARGET
add_executable(alcove main.cpp)
target_include_directories(alcove PUBLIC include)
target_link_libraries(alcove libraries)