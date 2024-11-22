#pragma once

#include "asset-importer/importer/importer.h"

class ShaderImporter : public BaseImporter {
  using_as_super(BaseImporter);
  
  uncopyable(ShaderImporter);

  ShaderImportData _importData;

public:

  ShaderImporter();

private:

  bool SetImportData_Impl(const BaseImportData* data) override;

  bool ProcessImportData_Impl() override;

  bool GenerateMetaData_Impl() override;

  bool Import_Impl() override;

};
