#pragma once

#include "core/uuid/uuid.h"
#include "core/types/constants.h"
#include "core/types/handle.h"

#include "core/utils/macros.h"

#include <vector>

enum class ResourceType {
  Unknown,

  Shader,

  Model,
  Mesh,
  Material,
  Texture,

  Animator,
  Animation,

  Audio,
};

enum AL_HANDLE_TYPE : u8 {
  AL_HANDLE_UNKNOWN,
  AL_HANDLE_SHADER,
  AL_HANDLE_MODEL,
  AL_HANDLE_MESH,
  AL_HANDLE_MATERIAL,
  AL_HANDLE_TEXTURE,
  AL_HANDLE_ANIMATOR,
  AL_HANDLE_ANIMATION,
  AL_HANDLE_AUDIO,

  AL_INVALID_HANDLE_TYPE = 0xFF
};

inline void SetHandleType(Handle& handle, AL_HANDLE_TYPE type, u8 spec) {
  std::size_t stype = type;
  std::size_t sspec = spec;
  handle.SetDesc((stype << 8) | sspec);
}

inline AL_HANDLE_TYPE GetHandleType(const Handle& handle) {
  std::size_t desc = handle.desc;
  desc = desc >> 8;
  return (AL_HANDLE_TYPE) static_cast<u8>(desc);
}

struct ResourceBase {
  UUID _uuid;
  char _name[MAX_NAME_LENGHT];
  char _path[MAX_PATH_LENGHT];
  char _exportedPath[MAX_PATH_LENGHT];
};
