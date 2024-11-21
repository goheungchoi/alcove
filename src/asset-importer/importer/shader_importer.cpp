#include "asset-importer/importer/shader_importer.h"

ShaderImporter::ShaderImporter(const ShaderImportSetting *setting)
: BaseImporter(AssetType::Shader, setting),
  _setting{*setting} {}


void ShaderImporter::Import(const char *path)
{
}
