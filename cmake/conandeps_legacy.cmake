message(STATUS "Conan: Using CMakeDeps conandeps_legacy.cmake aggregator via include()")
message(STATUS "Conan: It is recommended to use explicit find_package() per dependency instead")

find_package(glm)
find_package(SDL2)
find_package(Catch2)
find_package(imgui)
find_package(fmt)
find_package(assimp)
find_package(nlohmann_json)
find_package(stduuid)
find_package(Ktx)

set(CONANDEPS_LEGACY  glm::glm  SDL2::SDL2main  Catch2::Catch2WithMain  imgui::imgui  fmt::fmt  assimp::assimp  nlohmann_json::nlohmann_json  stduuid::stduuid  KTX::ktx )