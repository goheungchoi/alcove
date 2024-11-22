#include "asset-importer/importer/shader_importer.h"

#include "core/utils/io_util.h"

#include <cstring>

ShaderImporter::ShaderImporter()
: BaseImporter(AssetType::Shader), _importData{}, _info{}, _options{} {}

bool ShaderImporter::SetImportData_Impl(const BaseImportData* data) {
   if (data->type != ImportDataStructType::SHADER_IMPORT_DATA_STRUCT)
    return false;

  auto* pShaderImportData = static_cast<ShaderImportData*>(data->pNext);
  _importData = *pShaderImportData; 
  return true;
}

bool ShaderImporter::ProcessImportData_Impl() { 
  
  strncpy(_info.name, _name, MAX_NAME_LENGHT);
  strncpy(_info.path, _path, MAX_NAME_LENGHT);

  char uuid[33]; uuid[32] = '\0';
  UUIDToString(_uuid, uuid);
  strncpy(_info.outc, uuid, MAX_NAME_LENGHT);

  _info.lang = _importData.lang;
  _info.langVersion = _importData.langVersion;
  _info.type = _importData.type;

  _info.spv = _importData.spv;

  _info.client = _importData.client;
  _info.clientVersion = _importData.clientVersion;

  _info.numDefines = _importData.numDefines;
  _info.defines = _importData.defines;

  if (!al::read_file(_path, &_info.size, (void**) &_info.data))
    return false;

  _options.numShaders = 1;
  _options.shaderInfos = &_info;

  strncpy(_options.outdir, _fullExportDir, MAX_PATH_LENGHT);

  // TODO: Apply the actual compile options
  _options.numFlags = 0;
  _options.flags = "";

  return false; 
  
}

bool ShaderImporter::GenerateMetaData_Impl() { return false; }

bool ShaderImporter::Import_Impl() { 
  
  
  return false; }
