# PROJECT: Alcove

## Developemnt Environment 

Must-be-installed Programs for Dev
- Any C++ Compiler [GCC, Clang, MSVC] version that supports C++20
- Git v2.34 or higher
- CMake v3.25 or higher
- Conan v2.00 or higher
- Vulcan SDK v1.3 or higher

## Current Build Environment

- Windows 
- Scripting Tool: Python
- Dependency manager: Conan
- Build Generator: CMake
- Build Tool: Microsoft Visual Studio 2019
- Compiler: MSVC

## Possible Build Environment

- Windows MinGW
- Scripting Tool: Unix Makefile 
- Dependency manager: Conan
- Build Generator: CMake
- Build Tool: Unix Makefile
- Compiler: GCC

## Alternative Options

- Windows
- Scripting Tool: NMAKE
- Dependency manager: Conan
- Build Generator: CMake
- Build Tool: Ninja
- Compiler: Clang

## How to Init
Windows
- Debug Mode:
  - `py pymake.py conan-debug`
  - `py pymake.py config-msvc`
  - `py pymake.py msvc-debug`

## Current Dependencies

- Imported by Conan
  - GLM [0.9.9.8]
  - SDL [2.30.3]
  - Catch2 [3.5.0]
  - imgui [1.90]
  - fmt [10.2.1]

## TODO List

- Shader Compiler Tests: Need to figure out how to test the shader compiler.
- Seems that debugMessenger must be placed in the vk_engine class.

- 8/29/24: Currently, the shaders are compiled when CMake is configured. Need a separate shader compiler 

## BUG REPORT

1. CMake `Vulkan_INCLUDE_DIRS` Overriding
- What happened?<br>
During the CMake configuration stage, the `Vulkan_INCLUDE_DIRS` variable keeps
being overridden as `{USER_DIR}/.conan2/p/vulkaa955012f78a68/p/include`.
- What should have happened?<br>
If the `FindVulkan()` function operated normally, the value of `Vulkan_INCLUDE_DIRS` must've been `$ENV{VULKAN_SDK}/Include`.
- What might be causing this issue?<br>
It seems that the `vulkan-headers` conan package is modifying the vulkan include path.
