#pragma once

namespace Tex {
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

enum class AlphaMode {
  Opaque,
  Transparent
};

struct ImageData {
  ColorSpace colorSpace;
  ValueType type;
  AlphaMode alphaMode;
};

// Compressed image options

enum class CompressionFormat {
  Default,
  BC1,
  BC1a,
  BC2,
  BC3,
  BC3n, // Normal format to improve quality by compressing normal (x, y, z) to (1, y, 0, x)
  BC4,  // One channel unsigned
  BC4S, // One channel signed
  BC5,  // Two channel unsigned
  BC5S, // Two channel signed
  BC6U,
  BC6S,
  BC7,
  Uncompressed,
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

struct CompressOptions {
  // TODO: Adjust format based on the channel.
  // Or, recommended settings.
  CompressionFormat format{ CompressionFormat::BC7 };
  CompressionQuality quality{ CompressionQuality::Normal };
  bool enableMipMap{ false };
  int numMipMaps{ 1 };  // if the value < 0 or > max, set to max
  MipMapFilter mipMapFilter{ MipMapFilter::Box };
  bool isNormalMap{ false };
  bool isCubeMap{ false };  // TODO: Support cube map
  bool useGPU{ true };
};

}

