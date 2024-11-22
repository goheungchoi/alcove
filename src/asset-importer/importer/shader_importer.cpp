#include "asset-importer/importer/shader_importer.h"

#include "core/utils/io_util.h"

ShaderImporter::ShaderImporter()
: BaseImporter(AssetType::Shader), _importData{} {}

bool ShaderImporter::SetImportData_Impl(const BaseImportData* data) {
  return false;
}

bool ShaderImporter::ProcessImportData_Impl() { return false; }

bool ShaderImporter::GenerateMetaData_Impl() { return false; }

bool ShaderImporter::Import_Impl() { 
  
  
  return false; }
