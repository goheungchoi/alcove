#pragma once

#include "asset-importer/importer/importer.h"

#include "asset-importer/shader-compiler/shader_compiler.h"

class ShaderImporter : public BaseImporter {
  using_as_super(BaseImporter);

  uncopyable(ShaderImporter);

  // Input import data
  ShaderImportData _importData;

  // Processed shader data
  ShaderInfo _info;
  // Processed shader compile options
  ShaderCompileOptions _options;

public:

  ShaderImporter();

private:

  bool SetImportData_Impl(const BaseImportData* data) override;

  bool ProcessImportData_Impl() override;

  bool GenerateMetaData_Impl() override;

  bool Import_Impl() override;

};
