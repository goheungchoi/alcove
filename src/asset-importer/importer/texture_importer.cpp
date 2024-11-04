#include "asset-importer/importer/texture_importer.h"

#include "asset-importer/compressor/texture_compressor.h"

TextureImporter::TextureImporter(const TextureImportSetting* setting)
: BaseImporter(AssetType::Texture, setting),
  _setting{*setting} {}

// TODO: Write .meta file?
void TextureImporter::Import(const char* _) {
  std::string tmp{ _path };
  std::size_t first = tmp.find_last_of('/') + 1;
  std::size_t last = tmp.find_last_of('.');

  std::string stem = tmp.substr(first, last - first);

  // TODO: Make a file directory named with the first two letters of UUID
  std::string exportPath = "lib/texture/";
  exportPath += stem + ".dds";

  TextureCompressor compressor;

  if (!compressor.Compress(_path, exportPath.c_str(), &_setting.options)) {
    // TODO: error message
    throw std::exception("TextureCompressor: Compression failed!");
  }
}

