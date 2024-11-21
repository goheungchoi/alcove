// shader_compiler_test.cpp
#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>

#include <filesystem>
#include <iostream>

#include "asset-importer/importer/texture_importer.h"

// Fixture for setting up a TextureImportData
TextureImportData CreateTextureImportSetting() {
  TextureImportData data{};
  data.colorSpace = TextureColorSpace::sRGB;
  data.valueType = TextureValueType::UINT8;
  data.alphaMode = TextureAlphaMode::Opaque;
  data.format = TextureCompressionFormat::BC7;
  data.quality = TextureCompressionQuality::Normal;
  data.enableMipMap = false;
  data.numMipMaps = 1;
  data.mipMapFilter = TextureMipMapFilter::Box;
  data.useGPU = true;
  
  return data;
}

TEST_CASE("TextureImporter Initialization", "[TextureImporter]") {
  std::filesystem::current_path( xstr(PROJECT_DIR) );
  
  
  TextureImportData data{ CreateTextureImportSetting() };
  
  BaseImportData baseData{
    .type = ImportDataStructType::SHADER_IMPORT_DATA_STRUCT,
    .pNext = &data,

    .name = "TestTexture",
    .path = "assets/texture/test_pic.png",
    .exportDir = "lib/resources"
  };
  
  TextureImporter importer;
  importer.SetImportData(&baseData).ProcessImportData();
    
  REQUIRE(std::string(importer.GetName()) == "TestTexture");
  REQUIRE(std::string(importer.GetPath()) == "assets/texture/test_pic.png");
}

TEST_CASE("TextureImporter Compression and MipMap Options", "[TextureImporter]") {
  std::filesystem::current_path( xstr(PROJECT_DIR) );
  
  TextureImportData data{ CreateTextureImportSetting() };
  data.enableMipMap = true;
  data.numMipMaps = 6;
  data.format = TextureCompressionFormat::BC7;
  data.quality = TextureCompressionQuality::Normal;
  
  BaseImportData baseData{
    .type = ImportDataStructType::SHADER_IMPORT_DATA_STRUCT,
    .pNext = &data,

    .name = "TestTexture",
    .path = "assets/texture/test_pic.png",
    .exportDir = "lib/resources"
  };

  TextureImporter importer;
  importer.SetImportData(&baseData).ProcessImportData().Import();

  REQUIRE(std::filesystem::exists(importer.GetExportPath()));
}

