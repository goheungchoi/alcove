#pragma once

#include "asset-importer/compressor/texture_options.h"

using namespace Tex;

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
  TextureCompressor() = default;

  bool Compress(const char* filename, const char* exportPath, const CompressOptions* settings);

private:
  struct Private;
  Private* _m{ nullptr };
};
