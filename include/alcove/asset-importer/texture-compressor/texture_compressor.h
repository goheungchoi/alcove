#pragma once

#include "core/types/texture_enums.h"

struct ImageData {
  bool isNormalMap{ false };
  bool isCubeMap{ false };  // TODO: Support cube map

  TextureColorSpace colorSpace;
  TextureValueType valueType;
  TextureAlphaMode alphaMode;
  
  // Supported only when isCubeMap is true
  TextureCubeLayout cubeLayout;
};

struct CompressionOptions {
  // TODO: Adjust format based on the channel.
  // Or, recommended settings.
  TextureCompressionFormat format;
  TextureCompressionQuality quality;

  // Mipmap settings
  bool enableMipMap;
  int numMipMaps{ 1 };  // if the value < 0 or > max, set to max
  TextureMipMapFilter mipMapFilter{ TextureMipMapFilter::Box };

  // Use GPU
  bool useGPU{ true };
};

/**
 * @brief Compress image files into BC6 or BC7 formats
 * 
 * Texture Compression Formats
 * S3TC/DXT - BC1, BC2, BC3. Legacy image compression.
 * RGTC - BC4 (1-channel), BC5 (2-channel) - Use for normal maps, metallic-rouphness map.
 * BPTC - BC6, Support for HDR. HDR light maps, environment maps, skyboxes, and specular/emissive textures.
 * BC7 - High quality color image compression. 
 * 
 * Channel configurations:
 * 1-Channel (Grayscale): Some compressed formats, like BC4, only compress a single channel. This is often used for data such as height maps or single-channel masks.
 * 2-Channel (RG): Formats like BC5 compress two channels, typically used for normal maps where only the red and green channels are needed.
 * 3-Channel (RGB): Some formats support compression of three channels without an alpha channel (e.g., DXT1/BC1).
 * 4-Channel (RGBA): Many formats, such as BC3 (DXT5) and BC7, support compression of all four channels, including alpha. These are commonly used for color textures that need transparency.
 * 
 * Examples:
 * BC1 (DXT1): Compressed RGB with optional 1-bit alpha. Suitable for opaque textures or textures with simple alpha masking.
 * BC2 (DXT3): Compressed RGBA with explicit alpha. It uses separate data blocks for RGB and alpha channels.
 * BC3 (DXT5): Compressed RGBA with interpolated alpha. Offers better alpha channel quality than BC2.
 * BC4: Compressed single-channel (R). Often used for height maps or single-channel textures.
 * BC5: Compressed two-channel (RG). Commonly used for normal maps.
 * BC7: Compressed RGBA with high quality and flexible encoding options. Ideal for high-quality PBR textures.
 */
class TextureCompressor {
  TextureCompressor(const TextureCompressor&) = delete;
  TextureCompressor& operator=(const TextureCompressor&) = delete;

public:
  TextureCompressor() = default;

  bool Compress(const char* filename, const char* exportPath, const ImageData* data, const CompressionOptions* settings);

  bool CompressKTX2(const char* filename, const char* exportPath, const ImageData* data, const CompressionOptions* settings);

  bool CompressCube(const char* filename, const char* exportPath, const ImageData* data, const CompressionOptions* settings);

};
