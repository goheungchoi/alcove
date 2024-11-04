#pragma once

struct UUID {
  unsigned char byte[16];

  UUID() : byte{} {}
  UUID(const unsigned char* data);
};

UUID GenerateUUID_v4();

UUID GenerateUUID_v5(const char* name);

bool IsValidUUID(const UUID& id);
bool IsValidUUID(const char* id);
bool IsNilUUID(const UUID& id);

void UUIDToString(const UUID& id, char* outString);
bool StringToUUID(const char* str, UUID* outId);

