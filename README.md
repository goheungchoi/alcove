# Alcove Engine

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
  - imgui [1.91.0]
  - fmt [10.2.1]
  - assimp [5.4.2]

## TODO:
- 8/29/24: Currently, the shaders are compiled when CMake is configured. Need a separate shader compiler 
- 9/01/24: The path to the shader directory is currently relative to the build directory. Need a convenient way to access the root project directory.

1. Resource manager
   1. Animations
   2. Audio (OGG, MP3, WAV, ...)
   3. Materials
   4. Models (FBX, OBJ, ...)
   5. Textures (DDS, TIF, PNG, TGA, JPG, ...)
   6. Prefabs
   7. Skybox?
  
  - .mata file uses json format
  - Use GUID

  - Importer: compress and import a file into another file. Or, split one file into multiple assets.
