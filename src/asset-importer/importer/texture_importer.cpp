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

void TextureImporter::Serialize() {
  Super::Serialize();

  SetData("image.is_normal", _data.isNormalMap);
  SetData("image.is_cube", _data.isCubeMap);

  // Color space
  char colorSpace[MAX_NAME_LENGHT];
  switch (_data.colorSpace)
  {
  case TextureColorSpace::sRGB:
    strncpy(colorSpace, "sRGB", MAX_NAME_LENGHT);
    break;
  case TextureColorSpace::Non_Color:
    strncpy(colorSpace, "Non_Color", MAX_NAME_LENGHT);
    break;
  case TextureColorSpace::Linear:
    strncpy(colorSpace, "Linear", MAX_NAME_LENGHT);
    break;
  case TextureColorSpace::XYZ:
    strncpy(colorSpace, "XYZ", MAX_NAME_LENGHT);
    break;
  }
  SetData("image.color_space", colorSpace);

  // Alpha mode
  char alphaMode[MAX_NAME_LENGHT];
  switch (_data.alphaMode)
  {
  case TextureAlphaMode::Opaque:
    strncpy(alphaMode, "Opaque", MAX_NAME_LENGHT);
    break;
  case TextureAlphaMode::Transparent:
    strncpy(alphaMode, "Transparent", MAX_NAME_LENGHT);
    break;
  }
  SetData("image.alpha_mode", alphaMode);

  // TODO: Serialize the rest of the data
  

}