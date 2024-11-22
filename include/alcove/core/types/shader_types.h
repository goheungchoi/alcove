#pragma once

#include "core/types/numeric_types.h"
#include "core/types/constants.h"

#include "core/utils/magic_num.h"

constexpr u32 SHADER_TYPE_HEADER_MAGIC_NUM = MAGIC('H','E','A','D');
constexpr u32 SHADER_TYPE_VERTEX_MAGIC_NUM = MAGIC('V','E','R','T');
constexpr u32 SHADER_TYPE_GEOMETRY_MAGIC_NUM = MAGIC('G','E','O','M');
constexpr u32 SHADER_TYPE_TESS_CONTROL_MAGIC_NUM = MAGIC('T','E','S','C');
constexpr u32 SHADER_TYPE_TESS_EVALUATION_MAGIC_NUM = MAGIC('T','E','S','E');
constexpr u32 SHADER_TYPE_FRAGMENT_MAGIC_NUM = MAGIC('F','R','A','G');
constexpr u32 SHADER_TYPE_COMPUTE_MAGIC_NUM = MAGIC('C','O','M','P');
constexpr u32 SHADER_TYPE_RAY_GEN_MAGIC_NUM = MAGIC('R','G','E','N');
constexpr u32 SHADER_TYPE_RAY_INTERSECT_MAGIC_NUM = MAGIC('R','I','N','T');
constexpr u32 SHADER_TYPE_RAY_ANY_HIT_MAGIC_NUM = MAGIC('R','A','H','T');
constexpr u32 SHADER_TYPE_RAY_CLOSEST_HIT_MAGIC_NUM = MAGIC('R','C','H','T');
constexpr u32 SHADER_TYPE_RAY_MISS_MAGIC_NUM = MAGIC('R','M','S','S');
constexpr u32 SHADER_TYPE_RAY_CALLABLE_MAGIC_NUM = MAGIC('R','C','L','L');
constexpr u32 SHADER_TYPE_MESH_MAGIC_NUM = MAGIC('M','E','S','H');
constexpr u32 SHADER_TYPE_TASK_MAGIC_NUM = MAGIC('T','A','S','K');


constexpr const char* SHADER_LANG_VERSION_VULKAN_1_0 = "1.0.xx";
constexpr const char* SHADER_LANG_VERSION_VULKAN_1_1 = "1.1.xx";
constexpr const char* SHADER_LANG_VERSION_VULKAN_1_2 = "1.2.xx";
constexpr const char* SHADER_LANG_VERSION_VULKAN_1_3 = "1.3.xx";
constexpr const char* SHADER_LANG_VERSION_OPENGL_450 = "450";


constexpr const char* SHADER_CLIENT_VERSION_SPV_1_0 = "1.0.xx";
constexpr const char* SHADER_CLIENT_VERSION_SPV_1_1 = "1.1.xx";
constexpr const char* SHADER_CLIENT_VERSION_SPV_1_2 = "1.2.xx";
constexpr const char* SHADER_CLIENT_VERSION_SPV_1_3 = "1.3.xx";
constexpr const char* SHADER_CLIENT_VERSION_SPV_1_4 = "1.4.xx";
constexpr const char* SHADER_CLIENT_VERSION_SPV_1_5 = "1.5.xx";
constexpr const char* SHADER_CLIENT_VERSION_SPV_1_6 = "1.6.xx";


enum class ShaderType : u32 {
  UNKNOWN = 0,

  HEADER = SHADER_TYPE_HEADER_MAGIC_NUM,

  VERTEX = SHADER_TYPE_VERTEX_MAGIC_NUM,
  GEOMETRY = SHADER_TYPE_GEOMETRY_MAGIC_NUM,
  TESS_CONTROL = SHADER_TYPE_TESS_CONTROL_MAGIC_NUM,
  TESS_EVALUATION = SHADER_TYPE_TESS_EVALUATION_MAGIC_NUM,
  FRAGMENT = SHADER_TYPE_FRAGMENT_MAGIC_NUM,

  COMPUTE = SHADER_TYPE_COMPUTE_MAGIC_NUM,

  RAY_GEN = SHADER_TYPE_RAY_GEN_MAGIC_NUM,
  RAY_INTERSECT = SHADER_TYPE_RAY_INTERSECT_MAGIC_NUM,
  RAY_ANY_HIT = SHADER_TYPE_RAY_ANY_HIT_MAGIC_NUM,
  RAY_CLOSEST_HIT = SHADER_TYPE_RAY_CLOSEST_HIT_MAGIC_NUM,
  RAY_MISS = SHADER_TYPE_RAY_MISS_MAGIC_NUM,
  RAY_CALLABLE = SHADER_TYPE_RAY_CALLABLE_MAGIC_NUM,

  MESH = SHADER_TYPE_MESH_MAGIC_NUM,
  TASK = SHADER_TYPE_TASK_MAGIC_NUM,

  INVALID = U32_MAX
};

enum class ShaderLanguage {
  GLSL,
  HLSL,
  ESSL
};

enum class ShaderCompileTarget {
  Spv_1_0 = 0x010000u,
  Spv_1_1 = 0x010100u,
  Spv_1_2 = 0x010200u,
  Spv_1_3 = 0x010300u,
  Spv_1_4 = 0x010400u,
  Spv_1_5 = 0x010500u,
  Spv_1_6 = 0x010600u,
};

enum class ShaderClientTarget {
  OpenGL, // Not supported
  Vulkan,
  DX11,   // Not supported
  DX12,   // Not supported
  Metal,  // Not supported
};

enum class ShaderDefineValueType : u8 {
  NONE,
  BOOL,
  INTEGER,
  FLOAT,
  STRING,
};

struct ShaderDefine {
  char name[MAX_NAME_LENGHT];     // 64 bytes
  char define[MAX_NAME_LENGHT];   // 64 bytes
  ShaderDefineValueType type;     // 1 bytes
  u8 numValues;                   // 1 bytes
  u8 valueData[MAX_DATA_LENGHT];  // 1024 bytes
};  // total: 1154 bytes

struct ShaderInfo {
  char name[MAX_NAME_LENGHT];
  char path[MAX_PATH_LENGHT];
  
  char outc[MAX_NAME_LENGHT];

  ShaderLanguage lang;
  const char* langVersion;
  ShaderType type;

  ShaderCompileTarget spv;

  ShaderClientTarget client;
  const char* clientVersion;

  size_t numDefines;
  ShaderDefine* defines;

  size_t size;
  const u8* data;

  const char* entryName;
};


