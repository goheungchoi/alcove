import os
import shutil

from conan import ConanFile
from conan.tools.cmake import CMakeToolchain, CMakeDeps
from conan.tools.env import VirtualBuildEnv, VirtualRunEnv
from conan.tools.scm import Git
from conan.tools.files import copy


class AlcoveRecipe(ConanFile):
  settings = "os", "compiler", "build_type", "arch"
  options = {"shared": [True, False], "fPIC": [True, False]}
  default_options = {"shared": False, "fPIC": True}

  def configure(self):
    if self.options.shared:
      # fPIC might have been removed in config_options(), so we use rm_safe
      self.options.rm_safe("fPIC")

  def requirements(self):
    self.requires("glm/0.9.9.8")
    self.requires("sdl/2.30.3")
    self.requires("catch2/3.5.0")
    self.requires("imgui/1.91.0")
    self.requires("fmt/10.2.1")

  def layout(self):
    self.folders.source = "src"
    self.folders.build = "build"
    self.folders.generators = "cmake"
    self.folders.root = "."

    # CPP_INFO Directories
    # self.cpp_info.includedirs = ['include']  # Ordered list of include paths
    # self.cpp_info.libdirs = ['lib']  # Directories where libraries can be found
    # self.cpp_info.bindirs = ['bin']  # Directories where executables and shared libs can be found

  def generate(self):
    # GENERATION PROCESS
    deps = CMakeDeps(self)
    deps.set_property("*", "cmake_find_mode", "both")
    deps.generate()
    tc = CMakeToolchain(self)
    tc.user_presets_path = "ConanPresets.json"
    tc.generate()
    benv = VirtualBuildEnv(self)
    benv.generate()
    renv = VirtualRunEnv(self)
    renv.generate()

  # TODO:
#   [imports]
# ./res/bindings, imgui_impl_glfw.cpp -> ../bindings
# ./res/bindings, imgui_impl_opengl3.cpp -> ../bindings
# ./res/bindings, imgui_impl_glfw.h -> ../bindings
# ./res/bindings, imgui_impl_opengl3.h -> ../bindings
  # def package(self):
  #   source_path = os.path.join(self.source_folder, "res/bindings") 
  #   dest_path = os.path.join(self.package_folder, "include/imgui/bindings")
  #   copy(self, "imgui_impl_sdl2.h", src=source_path, dst=dest_path)
  #   copy(self, "imgui_impl_sdl2.cpp", src=source_path, dst=dest_path)
  #   copy(self, "imgui_impl_vulkan.h", src=source_path, dst=dest_path)
  #   copy(self, "imgui_impl_vulkan.cpp", src=source_path, dst=dest_path)
