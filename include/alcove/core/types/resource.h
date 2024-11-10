#pragma once

#include "core/uuid/uuid.h"
#include "core/types/handle.h"

enum class ResourceType {
  Shader,

  Model,
  Mesh,
  Material,
  Texture,

  Animator,
  Animation,

  Audio,
};

constexpr size_t MAX_NAME_LENGHT{ 64 };
constexpr size_t MAX_PATH_LENGHT{ 256 };


struct ResourceBase {
  UUID _uuid;
  char _name[MAX_NAME_LENGHT];
  char _path[MAX_PATH_LENGHT];
  char _exportedPath[MAX_PATH_LENGHT];
};
