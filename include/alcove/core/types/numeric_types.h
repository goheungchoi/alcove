#pragma once

#include <cinttypes>
#include <cstddef>
#include <limits>

using u8 = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;

using size_t = std::size_t;

constexpr u8  U8_MAX  { std::numeric_limits<u8>::max()  };
constexpr u16 U16_MAX { std::numeric_limits<u16>::max() };
constexpr u32 U32_MAX { std::numeric_limits<u32>::max() };
constexpr u64 U64_MAX { std::numeric_limits<u64>::max() };
