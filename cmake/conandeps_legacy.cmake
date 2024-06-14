message(STATUS "Conan: Using CMakeDeps conandeps_legacy.cmake aggregator via include()")
message(STATUS "Conan: It is recommended to use explicit find_package() per dependency instead")

find_package(glm)
find_package(SDL2)
find_package(Catch2)
find_package(imgui)
find_package(fmt)

set(CONANDEPS_LEGACY  glm::glm  SDL2::SDL2main  Catch2::Catch2WithMain  imgui::imgui  fmt::fmt )