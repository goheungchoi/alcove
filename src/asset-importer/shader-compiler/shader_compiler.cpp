#include "asset-importer/shader-compiler/shader_compiler.h"

#include <shaderc/shaderc.hpp>

#include <string>
#include <sstream>

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

static bool CompileShaderTask(const ShaderInfo* info) {
  return false;
};

bool CompileShaders(const ShaderCompileOptions* option) {
  
  // SpvCompilationResult CompileGlslToSpv(const char* source_text,
  //                                       size_t source_text_size,
  //                                       shaderc_shader_kind shader_kind,
  //                                       const char* input_file_name,
  //                                       const char* entry_point_name,
  //                                       const CompileOptions& options)
  
  

  shaderc::Compiler compiler;

  shaderc::CompileOptions options;

  // compiler.CompileGlslToSpv()

  return false;
}

