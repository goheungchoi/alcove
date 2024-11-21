#include "core/uuid/uuid.h"

#include <uuid.h>

using namespace uuids;

class UUIDRandomGenerator {
  std::random_device rd;
  std::mt19937 generator;
  uuids::uuid_random_generator gen;
public:
  UUIDRandomGenerator() : generator{
    [this]() {
      std::array<int, std::mt19937::state_size> seed_data{};
      std::generate(std::begin(seed_data), std::end(seed_data), std::ref(rd));
      std::seed_seq seq(std::begin(seed_data), std::end(seed_data));
      return std::mt19937(seq);
    } ()
  }, gen{generator} {}
  UUIDRandomGenerator(const UUIDRandomGenerator&) = delete;
  UUIDRandomGenerator& operator=(const UUIDRandomGenerator&) = delete;

  uuid GenerateUUID() {
    return gen();
  }
};

UUID GenerateUUID_v4() {
  static UUIDRandomGenerator random_generator{};
  const uuid id = random_generator.GenerateUUID();
  return UUID(reinterpret_cast<const unsigned char*>(id.as_bytes().data()));
}

UUID GenerateUUID_v5(const char* name) {
  static uuid_name_generator gen(uuids::uuid::from_string("47183823-2574-4bfd-b411-99ed177d3e43").value());
  const uuid id = gen(name);
  return UUID(reinterpret_cast<const unsigned char*>(id.as_bytes().data()));
}

bool IsValidUUID(const UUID& id) {
  UUID& idd = const_cast<UUID&>(id);
  std::span sp(idd.byte);
  return uuid::is_valid_uuid(to_string(uuid(sp)));
}

bool IsValidUUID(const char* id) {
  auto uid = uuid::from_string(id);
  if (!uid) return false;
  return uuid::is_valid_uuid(to_string(*uid));
}

bool IsNilUUID(const UUID& id) {
  UUID& idd = const_cast<UUID&>(id);
  std::span sp(idd.byte);
  return uuid(sp).is_nil();
}

void UUIDToString(const UUID& id, char* outString) {
  UUID& idd = const_cast<UUID&>(id);
  std::span sp(idd.byte);
  std::string s{ to_string(uuid(sp)) };
  auto it = std::remove_if(s.begin(), s.end(), 
    [](char c) { return c == '-'; }
  );
  s.erase(it, s.end());

  std::span<char, 32> out(outString, 32);
  std::copy(s.begin(), s.end(), out.begin());
}

bool StringToUUID(const char* str, UUID* outId) {
  auto id = uuid::from_string(str);
  if (id) {
    std::span sp{ id->as_bytes() };
    memcpy(outId->byte, sp.data(), 16);
    return true;
  } else {
    memset(outId->byte, '\0', 16);
    return false;
  }
}

UUID::UUID(const unsigned char *data) {
  memcpy(byte, data, 16);
}

bool UUID::operator==(const UUID &other) const {
  return memcmp(byte, other.byte, 16) == 0;
}


