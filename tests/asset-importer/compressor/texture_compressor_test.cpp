// texture_compressor_test.cpp
#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>

#include <filesystem>
#include <iostream>
#include <algorithm>

#include "asset-importer/texture-compressor/texture_compressor.h"

#include "asset-importer/importer/importer_common.h"

#define xstr(a) str(a)
#define str(a) #a

struct TextureImportData {
  char name[MAX_NAME_LENGHT];
  char path[MAX_PATH_LENGHT];

  bool isNormalMap;
  bool isCubeMap; 

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

// Fixture for setting up a TextureImportSetting
TextureImportData CreateTextureImportSetting(const char* name, const char* path) {
  TextureImportData setting = {};
  strncpy(setting.name, name, MAX_NAME_LENGHT);
  strncpy(setting.path, path, MAX_PATH_LENGHT);
  setting.colorSpace = TextureColorSpace::sRGB;
  setting.valueType = TextureValueType::UINT8;
  setting.alphaMode = TextureAlphaMode::Opaque;
  setting.format = TextureCompressionFormat::BC7;
  setting.quality = TextureCompressionQuality::Normal;
  setting.enableMipMap = false;
  setting.numMipMaps = 1;
  setting.mipMapFilter = TextureMipMapFilter::Box;
  setting.useGPU = true;
  
  return setting;
}

constexpr const char* ASSET_PATH{"assets/texture/test_pic.png"};
constexpr const char* IMPORT_DIR{ "lib/resources/" };

TEST_CASE("Texture Compression", "[TextureCompressor]") {
  std::filesystem::current_path( xstr(PROJECT_DIR) );
  
  UUID _uuid;
  char _name[MAX_NAME_LENGHT];
  char _path[MAX_PATH_LENGHT];
  char _exportPath[MAX_PATH_LENGHT];

  TextureImportData _setting = CreateTextureImportSetting("TestTexture", ASSET_PATH);
  _uuid = GenerateUUID_v5(ASSET_PATH);
  strcpy(_name, "TestTexture");
  strcpy(_path, "assets/texture/test_pic.png");
  // Make a file directory named with the first two letters of UUID
  char cuuid[32];
  UUIDToString(_uuid, cuuid);
  std::string str_uuid(cuuid, 32);

  std::string exportDir = IMPORT_DIR + str_uuid.substr(0, 2);
  std::filesystem::create_directory(exportDir);

  std::string exportPath = exportDir + "/" + str_uuid;
  strcpy(_exportPath, exportPath.c_str());

  ImageData data{  
    false, false,
    _setting.colorSpace, 
    _setting.valueType, 
    _setting.alphaMode
  };

  CompressionOptions options{
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
  REQUIRE(compressor.CompressKTX2(_path, _exportPath, &data, &options));
}

