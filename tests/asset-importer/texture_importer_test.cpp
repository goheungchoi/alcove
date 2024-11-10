// shader_compiler_test.cpp
#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>

#include <filesystem>
#include <iostream>

#include "asset-importer/importer/texture_importer.h"

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

TEST_CASE("TextureImporter Initialization", "[TextureImporter]") {
  std::filesystem::current_path( xstr(PROJECT_DIR) );
  
  TextureImportSetting setting = CreateTextureImportSetting("TestTexture", "assets/texture/test_pic.png");
  TextureImporter importer(&setting);

  REQUIRE(std::string(importer.GetName()) == "TestTexture");
  REQUIRE(std::string(importer.GetPath()) == "assets/texture/test_pic.png");
}

TEST_CASE("TextureImporter Compression and MipMap Options", "[TextureImporter]") {
  std::filesystem::current_path( xstr(PROJECT_DIR) );
  
  TextureImportSetting setting = CreateTextureImportSetting("TestTexture", "assets/texture/test_pic.png");
  setting.options.enableMipMap = true;
  setting.options.numMipMaps = 6;
  setting.options.format = Tex::CompressionFormat::BC7;
  setting.options.quality = Tex::CompressionQuality::Normal;

  TextureImporter importer(&setting);
  importer.Import("lib/texture/");

  REQUIRE(std::filesystem::exists(importer.GetExportPath()));
}

