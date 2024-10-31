#pragma once

#include "resource-manager/resource/resource.h"

class Texture : public ResourceBase {

public:
  Texture(const UUID& uuid, const char* name, const char* path, const char* exportedPath);

  void Load() override;
  void Unload() override;
};

