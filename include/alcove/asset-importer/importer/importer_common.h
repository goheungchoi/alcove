#pragma once

#include <memory>
#include <optional>
#include <string>
#include <vector>
#include <span>
#include <array>
#include <set>
#include <unordered_set>
#include <map>
#include <unordered_map>
#include <deque>
#include <functional>
#include <type_traits>
#include <algorithm>
#include <filesystem>

#include "core/uuid/uuid.h"

constexpr size_t MAX_NAME_LENGHT{ 64 };
constexpr size_t MAX_PATH_LENGHT{ 256 };

#define xstr(a) str(a)
#define str(a) #a

namespace std {
  inline void strip(std::string& str, char target = ' ') {
    auto it = std::remove_if(str.begin(), str.end(), 
      [target](char c) { return c == target; }
    );
    str.erase(it, str.end());
  }
}
