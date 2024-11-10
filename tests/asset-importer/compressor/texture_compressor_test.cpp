// texture_compressor_test.cpp
#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>

#include <filesystem>
#include <iostream>

#include "asset-importer/compressor/texture_compressor.h"

#include "asset-importer/importer/importer_common.h"

#define xstr(a) str(a)
#define str(a) #a

struct TextureImportSetting {
  char name[MAX_NAME_LENGHT];
  char path[MAX_PATH_LENGHT];
  Tex::ColorSpace colorSpace;
  Tex::ValueType valueType;
  Tex::AlphaMode alphaMode;
  Tex::CompressOptions options;
};

// Fixture for setting up a TextureImportSetting
TextureImportSetting CreateTextureImportSetting(const char* name, const char* path) {
  TextureImportSetting setting = {};
  strncpy(setting.name, name, MAX_NAME_LENGHT);
  strncpy(setting.path, path, MAX_PATH_LENGHT);
  setting.colorSpace = Tex::ColorSpace::sRGB;
  setting.valueType = Tex::ValueType::UINT8;
  setting.alphaMode = Tex::AlphaMode::Opaque;
  setting.options.format = Tex::CompressionFormat::BC7;
  setting.options.quality = Tex::CompressionQuality::Normal;
  setting.options.enableMipMap = false;
  setting.options.numMipMaps = 1;
  setting.options.mipMapFilter = Tex::MipMapFilter::Box;
  setting.options.isNormalMap = false;
  setting.options.isCubeMap = false;
  setting.options.useGPU = true;
  
  return setting;
}

constexpr const char* IMPORT_DIR{ "lib/resources/" };

TEST_CASE("Texture Compression", "[TextureCompressor]") {
  std::filesystem::current_path( xstr(PROJECT_DIR) );
  
  UUID _uuid;
  char _name[MAX_NAME_LENGHT];
  char _path[MAX_PATH_LENGHT];
  char _exportPath[MAX_PATH_LENGHT];

  TextureImportSetting _setting = CreateTextureImportSetting("TestTexture", "assets/texture/test_pic.png");
  strcpy(_name, "TestTexture");
  strcpy(_path, "assets/texture/test_pic.png");
  // Make a file directory named with the first two letters of UUID
  char cuuid[36];
  UUIDToString(_uuid, cuuid);
  std::string str_uuid(cuuid, 36);
  std::string exportDir = IMPORT_DIR + str_uuid.substr(0, 2);
  std::filesystem::create_directory(exportDir);

  std::string exportPath = exportDir + "/" + str_uuid;
  strcpy(_exportPath, exportPath.c_str());

  ImageData data{  
    _setting.colorSpace, 
    _setting.valueType, 
    _setting.alphaMode
  };
  TextureCompressor compressor;
  REQUIRE(compressor.CompressKTX2(_path, _exportPath, &data, &_setting.options));
}

