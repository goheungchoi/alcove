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
  virtual void Import(const char* path) = 0;
};

class BaseImporter : public IImporter {
protected:
  AssetType _type;
  UUID _uuid;
  char _name[MAX_NAME_LENGHT];
  char _path[MAX_PATH_LENGHT];
  char _exportPath[MAX_PATH_LENGHT];
public:
  BaseImporter(AssetType type, const BaseImportSetting* setting);
  AssetType GetAssetType() const { return _type; }
  UUID GetUUID() const { return _uuid; }
  const char* GetName() const { return _name; }
  const char* GetPath() const { return _path; }
  const char* GetExportPath() const { return _exportPath; }
};
