import os
import sys
import shutil
import argparse
import subprocess

from typing import Callable

from python.file_organizer import clean_generator_dir, organize_generator_dir

# Constants
GENERATOR = "Unix Makefiles"
CMAKE_CONFIG = f"-S . -B build -DCMAKE_EXPORT_COMPILE_COMMANDS=1 -G{GENERATOR} -DCMAKE_TOOLCHAIN_FILE=cmake/conan_toolchain.cmake"

def run_cmd(command:str, dir=None):
  try:
    # Change to the specified directory if any
    if dir is not None:
      # Get the current working directory
      curr_dir = os.getcwd()
      os.chdir(dir)
    
    # Execute the command
    result = subprocess.run(command, shell=True, check=True)

    # Change back to the current working directory
    if dir is not None:
      os.chdir(curr_dir)

    return result
  
  except subprocess.CalledProcessError as e:
    print(f"Error executing command: {e}", file=sys.stderr)
    sys.exit(1)

##################################
##### Clean Command ##############
##################################
def clean():
  if os.path.exists("build"):
    shutil.rmtree("build")
  os.mkdir("build")

##################################
##### Conan Install Command ######
##################################
def conan_debug():
  clean_generator_dir(current_workspace_directory=os.getcwd())
  run_cmd(f"conan install conanfile.py --build=missing --profile=debug")
  organize_generator_dir(current_workspace_directory=os.getcwd())

def conan_release():
  clean_generator_dir(current_workspace_directory=os.getcwd())
  run_cmd(f"conan install conanfile.py --build=missing --profile=release")
  organize_generator_dir(current_workspace_directory=os.getcwd())

##################################
##### Config Targets #############
##################################
def config_msvc():
  run_cmd(f"cmake -S . -B build -G \"Visual Studio 17 2022\" -DCMAKE_EXPORT_COMPILE_COMMANDS=1 -DCMAKE_TOOLCHAIN_FILE=cmake/conan_toolchain.cmake")

def config_test():
  run_cmd(f"cmake {CMAKE_CONFIG} -DCMAKE_BUILD_TYPE=Debug -DBUILD_TESTING=ON")

def config_debug():
  run_cmd(f"cmake {CMAKE_CONFIG} -DCMAKE_BUILD_TYPE=Debug -DBUILD_TESTING=OFF")

def config_release():
  run_cmd(f"cmake {CMAKE_CONFIG} -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTING=OFF")

##################################
##### Utility Targets ############
##################################
def graph():
  config_debug()
  if not os.path.exists("build"):
    print(f"Error build directory doesn't exist")
    exit(1)
  # Run the cmake command to generate the graph
  run_cmd("cmake .. --graphviz=graph.dot", dir='build')

  # Run the dot command to create the SVG file
  run_cmd("dot -Tsvg graph.dot -o dependency_graph.svg", dir='build')

##################################
##### Quick Build Targets ########
##################################
def is_cmake_configured():
  return os.path.exists("build") and os.path.exists("build/CMakeCache.txt")

def quick_build_decorator(target: Callable):
  def check_cmake_config():
    if is_cmake_configured():
      print("CMakeCache.txt is found in the build directory.")
      print("Build target...")
      target()
    else:
      print("CMakeCache.txt is not found in the build directory.")
      print("Please, make sure to run CMake configuration commands first.")
  return check_cmake_config

# MSVC Build Targest
@quick_build_decorator
def msvc_release():
  run_cmd("cmake --build build --config Release")

@quick_build_decorator
def msvc_debug():
  run_cmd("cmake --build build --config Debug")

# GCC, Clang Build Targets
@quick_build_decorator
def quick_build():
  run_cmd("cmake --build build")

##################################
##### Clean Build Targets ########
##################################
# MSVC Build Targets
def clean_msvc_release():
  clean()
  config_msvc()
  run_cmd("cmake --build build --config Release")

def clean_msvc_debug():
  clean()
  config_msvc()
  run_cmd("cmake --build build --config Debug")

# GCC, Clang Build Targets
def build_release():
  clean()
  config_release()
  run_cmd("cmake --build build")

def build_debug():
  clean()
  config_debug()
  run_cmd("cmake --build build")

def build_verbose():
  clean()
  config_debug()
  run_cmd("cmake --build build --verbose")

def build_test():
  clean()
  config_test()
  run_cmd("cmake --build build")

##################################
##### Test Targets ###############
##################################
# MSVC Test Targets  
def ctest_msvc_release():
  run_cmd("ctest -C Release -VV", dir='build')

def ctest_msvc_debug():
  run_cmd("ctest -C Debug -VV", dir='build')

# GCC, Clang Test Targets
def ctest():
  run_cmd("ctest -VV", dir='build')

##################################
##### MAIN #######################
##################################
def main():
  parser = argparse.ArgumentParser(prog="PyMake", description="System-agnostic build script")
  parser.add_argument("target", 
                      choices=[
                        # Clean
                        "clean", 
                        # Conan Install
                        "conan-debug", 
                        "conan-release",
                        # CMake Config
                        "config-msvc",
                        "config-test",
                        "config-debug",
                        "config-release",
                        # Util Targets
                        "graph",
                        # Quick Build Targets
                        "msvc-release",
                        "msvc-debug",
                        "quick-build",
                        # Clean Build Targets
                        "clean-msvc-release",
                        "clean-msvc-debug",
                        "build-release",
                        "build-debug",
                        "build-verbose",
                        "build-test",
                        # Test Targets
                        "ctest-msvc-release",
                        "ctest-msvc-debug",
                        "ctest",
                      ],
                      help="build helpers")

  args = parser.parse_args()

  # Clean
  if args.target == "clean":
    clean()

  # Conan Install
  elif args.target == "conan-debug":
    conan_debug()
  elif args.target == "conan-release":
    conan_release()

  # CMake Config
  elif args.target == "config-msvc":
    config_msvc()
  elif args.target == "config-test":
    config_test()
  elif args.target == "config-debug":
    config_debug()
  elif args.target == "config-release":
    config_release()

  # Util Targets
  elif args.target == "graph":
    graph()

  # Quick Build Targets
  elif args.target == "msvc-release":
    msvc_release()
  elif args.target == "msvc-debug":
    msvc_debug()
  elif args.target == "quick-build":
    quick_build()

  # Clean Build Targets
  ## MSVC Build Targets
  elif args.target == "clean-msvc-release":
    clean_msvc_release()
  elif args.target == "clean-msvc-debug":
    clean_msvc_debug()
  ## GCC, Clang Build Targets
  elif args.target == "build-release":
    build_release()
  elif args.target == "build-debug":
    build_debug()
  elif args.target == "build-verbose":
    build_verbose()
  elif args.target == "build-test":
    build_test()

  # Test Targets
  elif args.target == "ctest-msvc-release":
    ctest_msvc_release()
  elif args.target == "ctest-msvc-debug":
    ctest_msvc_debug()
  elif args.target == "ctest":
    ctest()
  
  # Unknown Targets
  else:
    print("ERR unknown target")
    parser.print_help()

if __name__ == "__main__":
  main()