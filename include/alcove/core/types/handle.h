#pragma once

#include <cinttypes>
#include <cstddef>
#include <limits>

using u8 = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;

union Handle {
public:

  static constexpr std::size_t NUM_INDEX_BITS{ 36 };
  static constexpr std::size_t NUM_VERSION_BITS{ 16 };
  static constexpr std::size_t NUM_DESC_BITS{ 16 };

  static constexpr u64 MIN_VERSION{ 0 };
  static constexpr u64 MAX_VERSION{ (1ULL << NUM_VERSION_BITS) - 2U };

  static constexpr u64 MAX_INDICES{ (1ULL << NUM_INDEX_BITS) - 2U };

  static constexpr u64 INVALID_HANDLE{ std::numeric_limits<u64>::max() };

private:
  u64 _value;

public:
  struct {
    u64 index : NUM_INDEX_BITS;
    u64 version : NUM_VERSION_BITS;
    u64 desc : NUM_DESC_BITS;
  };

  Handle() {}
  Handle(u64 value) : _value{ value } {}
  Handle(u64 index, u64 version, u64 desc)
  : index{ index }, version{ version }, desc{ desc } {}

  inline operator u64() const { return _value; }

};
