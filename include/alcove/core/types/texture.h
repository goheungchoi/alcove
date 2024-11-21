#pragma once

#include "core/types/resource.h"

#include "core/types/texture_enums.h"

struct MipmapData {
  u32 level;
  u32 size;
  std::vector<u8> data;
};

struct Texture : public ResourceBase {
  bool sRGB;
  bool isCubeMap;

  TextureAlphaMode alphaMode;

  uint32_t width;
  uint32_t height;

  TextureValueType valueType;
  TextureFormat format;

  uint32_t mipLevels;
  uint32_t arrayLayers;

  std::vector<MipmapData> mipmaps;
};



