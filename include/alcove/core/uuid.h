#pragma once

struct UUID {
  unsigned char byte[16];

  UUID() : byte{} {}
  UUID(const unsigned char* data);
};

inline UUID GenerateUUID_v4();

inline UUID GenerateUUID_v5(const char* name);

inline bool IsValidUUID(const UUID& id);
inline bool IsValidUUID(const char* id);
inline bool IsNilUUID(const UUID& id);

inline void UUIDToString(const UUID& id, char* outString);
inline bool StringToUUID(const char* str, UUID* outId);

