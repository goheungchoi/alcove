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
  GenerateMetaData_Impl();
  return *this;
}

void BaseImporter::Import() {
  // Create the export directory
  std::filesystem::create_directory(_fullExportDir);

  // Import process implementation
  Import_Impl();
}

void BaseImporter::Serialize() {
  // Asset type
  char assetType[MAX_NAME_LENGHT];
  switch (_type)
  {
  case AssetType::Shader:
    strncpy(assetType, "Shader", MAX_NAME_LENGHT);
    break;
  case AssetType::Model:
    strncpy(assetType, "Model", MAX_NAME_LENGHT);
    break;
  case AssetType::Mesh:
    strncpy(assetType, "Mesh", MAX_NAME_LENGHT);
    break;
  case AssetType::Material:
    strncpy(assetType, "Material", MAX_NAME_LENGHT);
    break;
  case AssetType::Texture:
    strncpy(assetType, "Texture", MAX_NAME_LENGHT);
    break;
  case AssetType::Animator:
    strncpy(assetType, "Animator", MAX_NAME_LENGHT);
    break;
  case AssetType::Animation:
    strncpy(assetType, "Animation", MAX_NAME_LENGHT);
    break;
  case AssetType::Audio:
    strncpy(assetType, "Audio", MAX_NAME_LENGHT);
    break;
  default:
    strncpy(assetType, "Unknown", MAX_NAME_LENGHT);
    break;
  }
  SetData("asset_type", assetType);
  
  // UUID
  char strUUID[33]; strUUID[32] = '\0';
  UUIDToString(_uuid, strUUID);
  SetData("uuid", strUUID);

  // Name
  SetData("name", _name);

  // Path
  SetData("path", _path);
  
  // Export path
  SetData("export_path", _fullExportPath);
}
