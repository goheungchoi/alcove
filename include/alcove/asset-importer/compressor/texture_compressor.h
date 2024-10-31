#pragma once

#include "asset-importer/compressor/compression_types.h"

/**
 * @brief Compress image files into BC6 or BC7 formats
 * 
 * Texture Compression Formats
 * S3TC/DXT - BC1, BC2, BC3. Legacy image compression.
 * RGTC - BC4, BC5 - Use for normal maps, metallic-rouphness map.
 * BPTC - BC6, Support for HDR. HDR light maps, environment maps, skyboxes, and specular/emissive textures.
 * BC7. High quality color image compression. 
 */
class TextureCompressor {
  TextureCompressor(const TextureCompressor&) = delete;
  TextureCompressor& operator=(const TextureCompressor&) = delete;

public:
  TextureCompressor();
  ~TextureCompressor();

  bool Load(const char* filename, ValueType type = ValueType::UINT8, int channels = 4);

  bool Compress(const char* filename, const CompressSettings* settings);

  bool Export(const char* path);

private:
  struct Private;
  Private* _m{ nullptr };
};
