#include "resource-manager/resource/resource.h"

#include <cstring>

ResourceBase::ResourceBase(
  const UUID& uuid, 
  const char* name, 
  const char* path, 
  const char* exportedPath) 
  : _uuid{ uuid } {
  strcpy(_name, name);
  strcpy(_path, path);
  strcpy(_exportedPath, exportedPath);
}
