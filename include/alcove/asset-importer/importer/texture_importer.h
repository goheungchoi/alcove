#pragma once

#include "asset-importer/importer/importer.h"

#include "asset-importer/compressor/texture_options.h"

struct TextureImportSetting : public BaseImportSetting {
  Tex::ValueType valueType;
  int channels;
  Tex::CompressOptions options;
};

class TextureImporter : public BaseImporter {
  TextureImportSetting _setting;
public:

  TextureImporter(const TextureImportSetting* setting);

  void Import(const char* path) override;

};
