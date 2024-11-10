#pragma once

#include "core/types/texture.h"

#include <memory>
#include <vector>
#include <queue>
#include <unordered_set>

template<typename T>
class ResourceCache {
  static constexpr std::size_t GROW_SIZE{ 1024 };

  std::queue<std::size_t> _emptySlotQueue;

  std::unordered_set<UUID> _uuids;

  std::vector<Handle> _handleTable;
  std::vector<u32> _refCounts;
  std::vector<T> _cacheTable;

public:
  
  Handle Load(const char* path) {
    UUID uuid;
    StringToUUID(path, &uuid);

    if (_uuids.find(uuid) == _uuids.end()) {
      // New resource 

      // Claim a handle

      // 


    } else {

    }


  }

  void Unload(Handle handle) {

  }

  const T& GetResource(Handle handle) {
    if (!IsValidHandle(handle))
      throw std::exception("Invalid handle!");  
    return _cacheTable[handle.index];
  }

private:
  void IncreaseCapacity() {
    std::size_t oldsize = _handleTable.size();
    if (oldsize >= Handle::MAX_INDICES) {
      throw std::exception("ResourceCache: Max. resource capacity reached!");
    }

    std::size_t newsize = oldsize + GROW_SIZE;
    newsize = (newsize < Handle::MAX_INDICES) ? newsize : Handle::MAX_INDICES;

    _handleTable.resize(newsize);
    _cacheTable.resize(newsize);
    _refCounts.resize(newsize);
  }

  bool IsValidHandle(Handle handle) {
    return handle.index < _handleTable.size() && 
      handle.version == _handleTable[handle.index].version;
  }

  Handle ClaimHandle(std::size_t nextslot) {

  }
};
