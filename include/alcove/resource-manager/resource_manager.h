#pragma once

#include "core/types/texture.h"

struct ResourceManager {

  Handle (*LoadTexture) (const char* path, TextureType textureType);
  void (*UnloadTexture) (Handle& handle);
  
  ResourceType (*GetResourceType) (const Handle& handle);
  bool (*IsValidHandle) (const Handle& handle);

  void (*UnloadAll) ();
};

const ResourceManager* GetResourceManager();

#define Al_LoadTexture(path, textureType) \
  GetResourceManager()->LoadTexture(path, textureType)

#define Al_UnloadTexture(handle)  \
  GetResourceManager()->UnloadTexture(handle)

#define Al_GetResourceType(handle)  \
  GetResourceManager()->GetResourceType(handle)

#define Al_IsValidHandle(handle)  \
  GetResourceManager()->IsValidHandle(handle);

