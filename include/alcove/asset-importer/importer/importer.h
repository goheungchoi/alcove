#pragma once

#include "asset-importer/importer/types.h"

#include "core/types/asset_types.h"

#include "core/env/file_paths.h"
#include "core/utils/macros.h"

#include "core/serializer/serializer.h"

/**
 * @brief 
 * 
 */
class IImporter : public Serializer {
public:
  virtual void Import() = 0;
};

class BaseImporter : public IImporter {
  uncopyable(BaseImporter);

  BaseImportData _baseData;

protected:
  AssetType _type;
  UUID _uuid;
  char _name[MAX_NAME_LENGHT];
  char _path[MAX_PATH_LENGHT];
  char _fullExportDir[MAX_PATH_LENGHT];
  char _fullExportPath[MAX_PATH_LENGHT];
public:
  BaseImporter(AssetType type);
  virtual ~BaseImporter() {};

  // Getters
  AssetType GetAssetType() const { return _type; }
  UUID GetUUID() const { return _uuid; }
  const char* GetName() const { return _name; }
  const char* GetPath() const { return _path; }
  const char* GetExportDir() const { return _fullExportDir; }
  const char* GetExportPath() const { return _fullExportPath; }

  // Retrieve the import data from the outside
  BaseImporter& SetImportData(const BaseImportData* data);

  BaseImporter& ProcessImportData();

  BaseImporter& GenerateMetaData();

  void Import() override final;

private:

  virtual bool SetImportData_Impl(const BaseImportData* data) = 0;

  virtual bool ProcessImportData_Impl() = 0;

  virtual bool GenerateMetaData_Impl() = 0;

  virtual bool Import_Impl() = 0;

};
