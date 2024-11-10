#pragma once

class AssetImporter {
  static constexpr size_t MAX_NAME_LENGHT{ 64 };
  static constexpr size_t MAX_PATH_LENGHT{ 256 };

  char textureAssetDir[MAX_PATH_LENGHT]{};
  char textureExportDir[MAX_PATH_LENGHT]{};

public:

  void SetTextureAssetDirectory(const char* dir);
  const char* GetTextureAssetDirectory() const;
  void SetTextureExportDirectory(const char* dir);
  const char* GetTextureExportDirectory() const;
  bool ImportTexture() const;


};
