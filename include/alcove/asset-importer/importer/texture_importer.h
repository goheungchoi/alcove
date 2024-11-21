#pragma once

#include "asset-importer/importer/importer.h"

#include "core/types/texture_enums.h"

struct TextureImportSetting : public BaseImportSetting {
  bool isNormalMap;
  bool isCubeMap;  // TODO: Support cube map

  TextureColorSpace colorSpace;
  TextureValueType valueType;
  TextureAlphaMode alphaMode;

  // Supported only when isCubeMap is true
  TextureCubeLayout cubeLayout;

  // Compression options
  TextureCompressionFormat format;
  TextureCompressionQuality quality;

  // Mipmap settings
  bool enableMipMap;
  int numMipMaps{ 1 };  // if the value < 0 or > max, set to max
  TextureMipMapFilter mipMapFilter{ TextureMipMapFilter::Box };

  // Use GPU
  bool useGPU{ true };
};

class TextureImporter : public BaseImporter {
  TextureImportSetting _setting;
public:

  TextureImporter(const TextureImportSetting* setting);

  void Import(const char* path) override;

};
