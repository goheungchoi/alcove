#pragma once

#include "resource-manager/resource_cache.h"

class ResourceManager {
  


public:

  Handle Load(const char* path);

  void Unload(Handle handle);

};

