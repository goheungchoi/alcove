#pragma once

// Uncompressed image options
enum class ValueType {
  UINT8,
  SINT8,
  FLOAT16,
  FLOAT32
};

enum class ColorSpace {
  sRGB,
  Non_Color,
  Linear,
  XYZ
};

// Compressed image options
enum class AlphaMode {
  Opaque,
  Transparent
};

enum class CompressionFormat {
  BC1,
  BC1A,
  BC2,
  BC3,
  BC3N,
  BC4,
  BC4S,
  BC5,
  BC5S,
  BC6U,
  BC6S,
  BC7
};

enum class CompressionQuality {
  Fast,
  Normal,
  Medium,
  Highest
};

enum class MipMapFilter {
  Box,
  Triangle,
  Kaiser,
  Mitchell,
  Min,
  Max
};

struct CompressSettings {
  CompressionFormat format{ CompressionFormat::BC7 };
  CompressionQuality quality{ CompressionQuality::Normal };
  ColorSpace colorSpace{ ColorSpace::sRGB };
  AlphaMode alphaMode{ AlphaMode::Opaque };
  bool enableMipMap{ false };
  int numMipMaps{ 1 };  // if the value < 0 or > max, set to max
  MipMapFilter mipMapFilter{ MipMapFilter::Box };
  bool isNormalMap{ false };
  bool isCubeMap{ false };  // TODO: Support cube map
  bool useGPU{ true };
};
