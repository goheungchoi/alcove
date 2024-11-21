#pragma once

#include "core/types/numeric_types.h"

union Handle {
public:

  static constexpr size_t NUM_INDEX_BITS{ 32 };
  static constexpr size_t NUM_VERSION_BITS{ 16 };
  static constexpr size_t NUM_DESC_BITS{ 16 };

  static constexpr u64 MIN_VERSION{ 0 };
  static constexpr u64 MAX_VERSION{ (1ULL << NUM_VERSION_BITS) - 2U };

  static constexpr u64 MAX_INDICES{ (1ULL << NUM_INDEX_BITS) - 2U };

  static constexpr u64 INVALID_HANDLE{ U64_MAX };

private:
  u64 _value;

  Handle(const Handle&) = delete;
  Handle& operator=(const Handle&) = delete;

public:
  struct {
    u64 index : NUM_INDEX_BITS;
    u64 version : NUM_VERSION_BITS;
    u64 desc : NUM_DESC_BITS;
  };

  Handle() : _value{ INVALID_HANDLE } {}
  Handle(u64 value) : _value{ value } {}
  Handle(u64 index, u64 version)
  : index{ index }, version{ version }, desc{} {}
  Handle(u64 index, u64 version, u64 desc)
  : index{ index }, version{ version }, desc{ desc } {}

  Handle(Handle&& other) noexcept {
    _value = other._value;
    other._value = INVALID_HANDLE;
  }
  Handle& operator=(Handle&& other) noexcept {
    _value = other._value;
    other._value = INVALID_HANDLE;
    return *this;
  }

  void SetDesc(u64 description) {
    desc = description;
  }

  inline operator u64() const { return _value; }

};
