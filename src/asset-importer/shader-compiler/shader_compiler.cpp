#include "asset-importer/shader-compiler/shader_compiler.h"

#include <shaderc/shaderc.hpp>

#include <string>
#include <sstream>
#include <fstream>

static shaderc_spirv_version GetSPIRV(ShaderCompileTarget spv) {
  return static_cast<shaderc_spirv_version>(spv);
}

static shaderc_env_version GetShaderClientVersion(ShaderClientTarget client, const char* version) {
  std::string v(version);
  std::istringstream ss(v);

  if (client == ShaderClientTarget::Vulkan) {
    std::string major;
    std::getline(ss, major, '.');
    std::string minor;
    std::getline(ss, minor, '.');

    u32 majorVersionNum{ std::stoul(major) };
    u32 minorVersionNum{ std::stoul(minor) };

    if (majorVersionNum == 1) {
      switch (minorVersionNum) {
        case 0:
          return shaderc_env_version_vulkan_1_0;
        case 1:
          return shaderc_env_version_vulkan_1_1;
        case 2:
          return shaderc_env_version_vulkan_1_2;
        case 3:
          return shaderc_env_version_vulkan_1_3;
      }
    }

    throw std::exception("Unsupported vulkan version used!");
  } else {
    throw std::exception("Unsupported shader client!");
  }
  
  return shaderc_env_version::shaderc_env_version_vulkan_1_0;
}

/**
 * @brief Read the binary data of a file. outData must be freed after use.
 * 
 * @param filepath 
 * @param outSize 
 * @param outData 
 * @return true 
 * @return false 
 */
static bool ReadFile(const char* filepath, size_t* outSize, u8** outData) {
  // Open the file with the cursor at the end
  std::ifstream file(filepath, std::ios::ate | std::ios::binary);

  if (!file.is_open()) {
    return false;
  }

  // The location of the cursor tells the size of 
  // the file in bytes
  std::size_t filesize = static_cast<std::size_t>(file.tellg());

  // Return the size of the file
  *outSize = filesize;
  
  if (outData) {
    // Allocate memory space for the byte data
    u8* data = (u8*) malloc(filesize);
    
    // Place the fie cursor at the beginning
    file.seekg(0);

    // Load the entire file into the buffer
    file.read((char*)data, filesize);

    *outData = data;
  }

  // Close the file
  file.close();

  // Return true
  return true;
}

bool CompileShaders(const ShaderCompileOption* option) {


  return false;
}

