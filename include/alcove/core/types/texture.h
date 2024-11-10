#pragma once

#include "core/types/resource.h"

enum class TextureValueType {
  UINT8,
  SINT8,
  FLOAT16,
  FLOAT32
};

enum class TextureAlphaMode {
  Opaque,
  Transparent
};

enum class TextureFormat {
  UNDEFINED,
  BC1,
  BC1A,
  BC2,
  BC3,
  BC4,
  BC5,
  BC6U,
  BC6S,
  BC7
};

struct Texture : public ResourceBase {
  bool _sRGB;

  TextureAlphaMode _alphaMode;

  uint32_t _width;
  uint32_t _height;

  TextureValueType _componentType;
  TextureFormat _format;

  uint32_t _mipLevels;
  uint32_t _arrayLevels;
};

