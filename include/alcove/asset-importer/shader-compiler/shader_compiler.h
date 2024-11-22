#pragma once

#include "core/types/shader_types.h"

struct ShaderCompileOptions {
  size_t numShaders;
  const ShaderInfo* shaderInfos;

  char outdir[MAX_PATH_LENGHT];

  size_t numFlags;
  const char* flags;

  bool parallel;
  size_t numThreads;
};

bool CompileShaders(const ShaderCompileOptions* option);
