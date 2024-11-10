#include "asset-importer/importer/importer.h"

#include <cstring>

BaseImporter::BaseImporter(AssetType type, const BaseImportSetting* setting) 
  : _type{ type } {
  strcpy(_name, setting->name);
  strcpy(_path, setting->path);
  _uuid = GenerateUUID_v5(_path);
}
