#pragma once

#include "asset-importer/importer/importer.h"

#include "asset-importer/texture-compressor/texture_compressor.h"

class TextureImporter : public BaseImporter {
  using_as_super(BaseImporter);

  uncopyable(TextureImporter);

  // Input import data
  TextureImportData _importData;

  // Processed image data
  ImageData _data;
  // Processed compression options
  CompressionOptions _options;

public:

  TextureImporter();

protected:

  void Serialize() override;

private:

  bool SetImportData_Impl(const BaseImportData* data) override;

  bool ProcessImportData_Impl() override;

  bool GenerateMetaData_Impl() override;

  bool Import_Impl() override;

};
