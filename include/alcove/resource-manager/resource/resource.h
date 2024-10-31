#pragma once

#include "core/uuid.h"

constexpr size_t MAX_NAME_LENGHT{ 64 };
constexpr size_t MAX_PATH_LENGHT{ 256 };

class IResource {
public:
  virtual ~IResource() = default;
  virtual void Load() = 0;
  virtual void Unload() = 0;
};

class ResourceBase : public IResource {
protected:
  UUID _uuid;
  char _name[MAX_NAME_LENGHT];
  char _path[MAX_PATH_LENGHT];
  char _exportedPath[MAX_PATH_LENGHT];

public:
  ResourceBase(const UUID& uuid, const char* name, const char* path, const char* exportedPath);
};
