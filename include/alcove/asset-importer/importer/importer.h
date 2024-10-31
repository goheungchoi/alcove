#pragma once

#include "asset-importer/importer/importer_common.h"

enum class AssetType {
  Shader,

  Model,
  Mesh,
  Material,
  Texture,

  Animator,
  Animation,

  Audio,
};

struct BaseImportSetting {
  char name[MAX_NAME_LENGHT];
  char path[MAX_PATH_LENGHT];
};

/**
 * @brief 
 * 
 */
class IImporter {
public:
  virtual void Import(std::filesystem::path path) = 0;
};

class BaseImporter : public IImporter {
protected:
  AssetType _type;
  char _name[MAX_NAME_LENGHT];
  char _path[MAX_PATH_LENGHT];

public:
  BaseImporter(const BaseImportSetting* setting);

};
