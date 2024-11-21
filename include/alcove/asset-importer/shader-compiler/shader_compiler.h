#pragma once

#include "core/types/shader_types.h"

struct ShaderCompileOption {
  size_t numShaders;
  const ShaderInfo* shaderInfos;

  char outdir[MAX_PATH_LENGHT];

  size_t numFlags;
  const char* flags;

  bool parallel;
};

bool CompileShaders(const ShaderCompileOption* option);

class ShaderCompiler {
  
public:



};
