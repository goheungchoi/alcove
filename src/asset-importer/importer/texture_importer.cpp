#include "asset-importer/importer/texture_importer.h"

#include "asset-importer/texture-compressor/texture_compressor.h"

TextureImporter::TextureImporter(const TextureImportSetting* setting)
: BaseImporter(AssetType::Texture, setting),
  _setting{*setting} {}

// TODO: Write .meta file?
void TextureImporter::Import(const char* importDir) {
  // Make a file directory named with the first two letters of UUID
  char cuuid[32];
  UUIDToString(_uuid, cuuid);
  std::string str_uuid(cuuid, 32);
  std::string exportDir = importDir + str_uuid.substr(0, 2);
  std::filesystem::create_directory(exportDir);

  // Set the export path
  std::string exportPath = exportDir + "/" + str_uuid;
  strcpy(_exportPath, exportPath.c_str());

  // Start compression
  ImageData data{ 
    .isNormalMap = _setting.isNormalMap,
    .isCubeMap = _setting.isCubeMap,

    .colorSpace = _setting.colorSpace, 
    .type = _setting.valueType, 
    .alphaMode = _setting.alphaMode,

    .cubeLayout = _setting.cubeLayout
  };
  CompressOptions options{
    .format = _setting.format,
    .quality = _setting.quality,

    // Mipmap settings
    .enableMipMap = _setting.enableMipMap,
    .numMipMaps = _setting.numMipMaps,
    .mipMapFilter = _setting.mipMapFilter,

    // Use GPU
    .useGPU = _setting.useGPU
  };
  TextureCompressor compressor;
  if (!compressor.CompressKTX2(_path, _exportPath, &data, &options)) {
    // TODO: error message
    throw std::exception("TextureCompressor: Compression failed!");
  }

  // TODO: Write .meta

}

