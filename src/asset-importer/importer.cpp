#include "asset-importer/importer/importer.h"

#include <cstring>

BaseImporter::BaseImporter(const BaseImportSetting* setting) {
  strcpy(_name, setting->name);
  strcpy(_path, setting->path);
}
