#pragma once

#include <functional>

struct UUID {
  unsigned char byte[16];

  UUID() : byte{} {}
  UUID(const unsigned char* data);

  bool operator==(const UUID& other) const;
};

namespace std {
  template <>
  struct hash<UUID> {
    std::size_t operator() (const UUID& uuid) const noexcept {
      std::size_t hash = 2166136261u; // FNV-1a 32-bit hash offset
      for (int i = 0; i < 16; ++i) {
        hash ^= uuid.byte[i];
        hash *= 16777619u; // FNV-1a prime
      }
      return hash;
    }
  };
}

UUID GenerateUUID_v4();

UUID GenerateUUID_v5(const char* name);

bool IsValidUUID(const UUID& id);
bool IsValidUUID(const char* id);
bool IsNilUUID(const UUID& id);

void UUIDToString(const UUID& id, char* outString);
bool StringToUUID(const char* str, UUID* outId);

