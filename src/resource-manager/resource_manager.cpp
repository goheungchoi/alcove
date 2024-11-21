#include "resource-manager/resource_manager.h"

#include "resource-manager/resource_pool.h"

struct __Private__ {
  ResourcePool<Texture> texturePool;


};

static bool isInitialized{ false };
static ResourceManager _rm{};
static __Private__ _m;

static Handle LoadTexture(const char *path, TextureType textureType)
{
  return _m.texturePool.Load(path, (void*) &textureType);;
}

static void UnloadTexture(Handle& handle)
{
  _m.texturePool.Unload(handle);
}

static ResourceType GetResourceType(const Handle& handle) {
  AL_HANDLE_TYPE type = GetHandleType(handle);
  switch (type) {
    case AL_HANDLE_SHADER:
      return ResourceType::Shader;
    case AL_HANDLE_MODEL:
      return ResourceType::Model;
    case AL_HANDLE_MESH:
      return ResourceType::Mesh;
    case AL_HANDLE_MATERIAL:
      return ResourceType::Material;
    case AL_HANDLE_TEXTURE:
      return ResourceType::Texture;
    case AL_HANDLE_ANIMATOR:
      return ResourceType::Animator;
    case AL_HANDLE_ANIMATION:
      return ResourceType::Animation;
    case AL_HANDLE_AUDIO:
      return ResourceType::Audio;
    default:
      return ResourceType::Unknown;
  }
}

static bool IsValidHandle(const Handle& handle) {
  AL_HANDLE_TYPE type = GetHandleType(handle);
  switch (type) {
    case AL_HANDLE_SHADER:
      return false;
    case AL_HANDLE_MODEL:
      return false;
    case AL_HANDLE_MESH:
      return false;
    case AL_HANDLE_MATERIAL:
      return false;
    case AL_HANDLE_TEXTURE:
      return _m.texturePool.IsValidHandle(handle);
    case AL_HANDLE_ANIMATOR:
      return false;
    case AL_HANDLE_ANIMATION:
      return false;
    case AL_HANDLE_AUDIO:
      return false;
    default:
      return false;
  }
}

static void UnloadAll() {
}

const ResourceManager* GetResourceManager() {
  if (!isInitialized) {
    _rm.LoadTexture = LoadTexture;
    _rm.UnloadTexture = UnloadTexture;
    _rm.GetResourceType = GetResourceType;
    _rm.IsValidHandle = IsValidHandle;
    _rm.UnloadAll = UnloadAll;

    isInitialized = true;
  }

  return &_rm;
}


