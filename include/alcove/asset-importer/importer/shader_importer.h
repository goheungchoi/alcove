#pragma once

#include "asset-importer/importer/importer.h"

struct ShaderImportSetting : public BaseImportSetting {
  
};

class ShaderImporter : public BaseImporter {
  ShaderImportSetting _setting;
public:
  ShaderImporter(const ShaderImportSetting* setting);

  void Import(const char* path) override;
};
