#include "asset-importer/importer/texture_importer.h"

#include "asset-importer/compressor/texture_compressor.h"

TextureImporter::TextureImporter(const TextureImportSetting* setting)
: BaseImporter(AssetType::Texture, setting),
  _setting{*setting} {}

// TODO: Write .meta file?
void TextureImporter::Import(const char* importDir) {
  // Make a file directory named with the first two letters of UUID
  char cuuid[36];
  UUIDToString(_uuid, cuuid);
  std::string str_uuid(cuuid, 36);
  std::string exportDir = importDir + str_uuid.substr(0, 2);
  std::filesystem::create_directory(exportDir);

  std::string exportPath = exportDir + "/" + str_uuid;
  strcpy(_exportPath, exportPath.c_str());

  ImageData data{  
    _setting.colorSpace, 
    _setting.valueType, 
    _setting.alphaMode
  };
  TextureCompressor compressor;
  if (!compressor.CompressKTX2(_path, _exportPath, &data, &_setting.options)) {
    // TODO: error message
    throw std::exception("TextureCompressor: Compression failed!");
  }

  // Write .meta

}

