#include "asset-importer/importer/texture_importer.h"

#include "asset-importer/texture-compressor/texture_compressor.h"

TextureImporter::TextureImporter()
: BaseImporter(AssetType::Texture), _importData{}, _data{}, _options{} {}

bool TextureImporter::SetImportData_Impl(const BaseImportData* data) {
  if (data->type != ImportDataStructType::TEXTURE_IMPORT_DATA_STRUCT)
    return false;

  auto* pTextureImportData = static_cast<TextureImportData*>(data->pNext);
  _importData = *pTextureImportData; 
  return true;
}

bool TextureImporter::ProcessImportData_Impl() {
  // Set image data
  _data.isNormalMap = _importData.isNormalMap;
  _data.isCubeMap = _importData.isCubeMap;

  _data.colorSpace = _importData.colorSpace;
  _data.valueType = _importData.valueType;
  _data.alphaMode = _importData.alphaMode;

  _data.cubeLayout = _importData.cubeLayout;

  // Set compression options
  _options.format = _importData.format;
  _options.quality = _importData.quality;

  _options.enableMipMap = _importData.enableMipMap;
  _options.numMipMaps = _importData.numMipMaps;
  _options.mipMapFilter = _importData.mipMapFilter;

  _options.useGPU = _importData.useGPU;

  return true;
}

bool TextureImporter::GenerateMetaData_Impl() {
  // TODO:
  return true;
}

bool TextureImporter::Import_Impl() { 
  
  TextureCompressor compressor;
  if (!compressor.CompressKTX2(_path, _fullExportPath, &_data, &_options)) {
    // TODO: error message
    throw std::exception("TextureCompressor: Compression failed!");
    return false;
  }

  return true;
}