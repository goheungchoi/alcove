#pragma once

#include "asset-importer/importer/importer_common.h"

#include "core/types/texture_enums.h"

enum class ImportDataStructType {
  INVALID_STRUCT = 0,

  BASE_IMPORT_DATA_STRUCT,
  TEXTURE_IMPORT_DATA_STRUCT,
  SHADER_IMPORT_DATA_STRUCT,
};

struct BaseImportData {
  ImportDataStructType type;
  void* pNext;

  char name[MAX_NAME_LENGHT];
  char path[MAX_PATH_LENGHT];
  char exportDir[MAX_PATH_LENGHT];
};

struct TextureImportData {
  bool isNormalMap;
  bool isCubeMap;  // TODO: Support cube map

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

struct ShaderImportData {


};

struct ModelImportData {


};

