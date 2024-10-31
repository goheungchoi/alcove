#pragma once

#include "resource-manager/resource/resource.h"

#include <memory>
#include <unordered_map>

template<typename T>
class ResourceCache {
  std::unordered_map<name, std::shared_ptr<T>> cache;

public:
  

};
