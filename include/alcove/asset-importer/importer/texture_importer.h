#pragma once

#include "asset-importer/importer/importer.h"

struct TextureImportSetting : public BaseImportSetting {

};

class TextureImporter : public BaseImporter {

public:

  TextureImporter(const TextureImportSetting* setting);

  void Import(std::filesystem::path path) override;

};
