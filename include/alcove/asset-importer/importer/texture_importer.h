#pragma once

#include "asset-importer/importer/importer.h"

#include "asset-importer/texture-compressor/texture_compressor.h"

class TextureImporter : public BaseImporter {
  uncopyable(TextureImporter);

  TextureImportData _importData;

  ImageData _data;
  CompressOptions _options;

public:

  TextureImporter();

private:

  bool SetImportData_Impl(const BaseImportData* data) override;

  bool ProcessImportData_Impl() override;

  bool GenerateMetaData_Impl() override;

  bool Import_Impl() override;

};
