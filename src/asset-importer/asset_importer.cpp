#include "asset-importer/asset_importer.h"

#include "asset-importer/importer/texture_importer.h"

#include <cstring>

void AssetImporter::SetTextureAssetDirectory(const char *dir)
{
}

const char *AssetImporter::GetTextureAssetDirectory() const
{
  return nullptr;
}

void AssetImporter::SetTextureExportDirectory(const char *dir)
{
}

const char *AssetImporter::GetTextureExportDirectory() const
{
  return nullptr;
}

bool AssetImporter::ImportTexture() const
{
  return false;
}
