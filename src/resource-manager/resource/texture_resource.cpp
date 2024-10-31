#include "resource-manager/resource/texture_resource.h"

Texture::Texture(
  const UUID& uuid, 
  const char* name, 
  const char* path, 
  const char* exportedPath)
  : ResourceBase(uuid, name, path, exportedPath) {}

void Texture::Load() {

}

void Texture::Unload() {

}
