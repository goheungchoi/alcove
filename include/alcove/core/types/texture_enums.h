#pragma once

#include "core/types/numeric_types.h"

/**
 * @brief Follows the NVTT fold and unfold cubemap layout rules.
 */
enum class TextureCubeLayout {
  Equirectangular,
  /// Unfolds into a 3*edgeLength (width) x 4*edgeLength texture, laid
  /// out as follows:
  /// ```text
  ///  2
  /// 140
  ///  3
  ///  5
  /// ```
  /// Face 5 is rotated 180 degrees.
  VerticalCross,
  /// Unfolds into a 4*edgeLength (width) x 3*edgeLength texture, laid
  /// out as follows:
  /// ```text
  ///  2
  /// 1405
  ///  3
  /// ```
  /// Face 5 is rotated 180 degrees.
  HorizontalCross,
  /// Writes each face in order into a column layout, like this:
  /// ```text
  /// 0
  /// 1
  /// 2
  /// 3
  /// 4
  /// 5
  /// ```
  Column,
  /// Writes each face in order into a row layout, like this:
  /// ```text
  /// 012345
  /// ```
  Row,
  /// Same as CubeLayout_VerticalCross.
  CubeLayout_LatitudeLongitude
};

// TODO: 
enum class TextureType : u8 {
  UNKNOWN = 0x00,
  DIFFUSE,
  SPECULAR,
  AMBIENT,
  EMISSIVE,
  HEIGHT,
  SHININESS,
  OPACITY,

  ALBEDO,
  NORMAL,
  METALIC,
  ROUGHNESS,
  METALIC_ROUGHNESS,
  AMBIENT_OCCLUSION,

  INVALID_TYPE = 0xFF
};

enum class TextureValueType {
  UINT8,
  SINT8,
  FLOAT32,
  FLOAT16
};

enum class TextureColorSpace {
  sRGB,
  Non_Color,
  Linear,
  XYZ
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
  BC3n,
  BC4,
  BC4S,
  BC5,
  BC5S,
  BC6U,
  BC6S,
  BC7,
  R16_FLOAT,
  R32_FLOAT,
  R16G16_FLOAT,
  R32G32_FLOAT,
  R16G16B16A16_FLOAT,
  R32G32B32A32_FLOAT
};


enum class TextureCompressionFormat {
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

enum class TextureCompressionQuality {
  Fast,
  Normal,
  Medium,
  Highest
};

enum class TextureMipMapFilter {
  Box,
  Triangle,
  Kaiser,
  Mitchell,
  Min,
  Max
};
