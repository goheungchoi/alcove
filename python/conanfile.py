from conan import ConanFile
from conan.tools.cmake import CMakeDeps, CMake, cmake_layout
from conan.tools.scm import Git

# [required]
# glm/0.9.9.8
# sdl/2.28.5
# catch2/3.5.0
# vulkan-memory-allocator/3.0.1

# imgui/1.9.0
# glslang/14.0.0

# [generators]
# CMakeDeps

# [layout]
# cmake_layout

class imguiRecipe(ConanFile):
  name = "imgui"
  version = "1.9.0"

  # Binary configuration
  settings = "os", "compiler", "build_type", "arch"
  options = {"shared": [True, False], "fPIC": [True, False]}
  default_options = {"shared": False, "fPIC": True}

  def source(self):
    git = Git(self)
    git.clone(url="https://github.com/ocornut/imgui.git", target="extern")
    git.checkout("v1.90")
  
  def config_options(self):
    if self.settings.os == "Windows":
      del self.options.fPIC

  def layout(self):
    cmake_layout(self)

  def generate(self):
    tc = CMakeDeps(self)
    tc.generate()

  def build(self):
    cmake = CMake(self)
    cmake.configure()
    cmake.build()

  def package(self):
    cmake = CMake(self)
    cmake.install()

  def package_info(self):
    self.cpp_info.libs = ["imgui"]

class glslangRecipe(ConanFile):
  name = "glslang"
  version = "14.0.0"

  # Binary configuration
  settings = "os", "compiler", "build_type", "arch"
  options = {"shared": [True, False], "fPIC": [True, False]}
  default_options = {"shared": False, "fPIC": True}

  def source(self):
    git = Git(self)
    git.clone(url="https://github.com/KhronosGroup/glslang.git", target="extern")
    git.checkout("14.0.0")
  
  def config_options(self):
    if self.settings.os == "Windows":
      del self.options.fPIC

  def layout(self):
    cmake_layout(self)

  def generate(self):
    tc = CMakeDeps(self)
    tc.generate()

  def build(self):
    cmake = CMake(self)
    cmake.configure()
    cmake.build()

  def package(self):
    cmake = CMake(self)
    cmake.install()

  def package_info(self):
    self.cpp_info.libs = ["glslang"]