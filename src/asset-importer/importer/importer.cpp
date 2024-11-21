#include "asset-importer/importer/importer.h"

#include "core/utils/string_util.h"

#include <cstring>

BaseImporter::BaseImporter(AssetType type) 
  : _type{ type }, _baseData{ ImportDataStructType::INVALID_STRUCT } {}

BaseImporter& BaseImporter::SetImportData(const BaseImportData* data) {
  if (data->type == ImportDataStructType::INVALID_STRUCT) 
    return *this;
  
  // Copy the base data
  _baseData = *data;

  // Retrieve the asset-specific import data
  if (!SetImportData_Impl(data))
    return *this;

  return *this;
}

BaseImporter& BaseImporter::ProcessImportData() { 
  if (_baseData.type == ImportDataStructType::INVALID_STRUCT)
    return *this;

  strcpy(_name, _baseData.name);
  strcpy(_path, _baseData.path);
  _uuid = GenerateUUID_v5(_path);

  // Set the full export directory
  char cuuid[32];
  UUIDToString(_uuid, cuuid);
  std::string str_uuid(cuuid, 32);
  std::string fullExportDir = al::text(_baseData.exportDir, "/", str_uuid.substr(0, 2));
  strcpy(_fullExportDir, fullExportDir.c_str());

  // Set the export path
  std::string exportPath = al::text(fullExportDir, "/", str_uuid);
  strcpy(_fullExportPath, exportPath.c_str());

  return *this;
}

BaseImporter& BaseImporter::GenerateMetaData() { 
  

  return *this;
}

void BaseImporter::Import() {
  // Create the export directory
  std::filesystem::create_directory(_fullExportDir);

  Import_Impl();
}
