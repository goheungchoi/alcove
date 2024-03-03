// shader_compiler.h
#pragma once

#include <glslang/Public/ShaderLang.h>

#include <iostream>
#include <string>
#include <vector>
#include <memory>

/**
 * @brief Compile shaders during runtime to generating SPIR-V. 
 * This eases application development phases.
 * Must be disabled for a release version. Instead, shaders must be pre-compiled.
 * 
 */
class GLSLCompiler {
// CONFIGURATION VARIABLES
static constexpr glslang::EShSource                  source_lang         = glslang::EShSourceGlsl;
static constexpr glslang::EShClient                  client              = glslang::EShClientVulkan;
static constexpr glslang::EShTargetClientVersion     client_version      = glslang::EShTargetVulkan_1_3;
static constexpr glslang::EShTargetLanguage          target_lang         = glslang::EShTargetSpv;
static constexpr glslang::EShTargetLanguageVersion   target_lang_version = glslang::EShTargetSpv_1_3;
  
public:
  GLSLCompiler(const std::string filename, bool output = false);
  ~GLSLCompiler();
  
  inline const std::vector<unsigned int>& getSpirvCode() const {
    return spir_v;
  }

private:
  std::vector<unsigned int> spir_v;

  void compileShader(const std::string& filename, bool output);

  using NamesTuple = std::tuple<EShLanguage, std::string>;
  const NamesTuple getStateNameAndBinaryNameFrom(const std::string& filename) const;
  const std::string readShaderFile(const std::string& filename) const;

  // TEST PURPOSE
  friend class GLSLCompilerTest;
};
