#pragma once

#include <concepts>

/**
 * @brief Returns a magic number representing a series of characters in big-endian.
 * For example, if 'T', 'E', 'X', and 'T' are inputted, an unsigned int whose bytes
 * correspond to 'T''X''E''T' will be returned.
 * 
 */
template <typename Ret = unsigned int, typename... Chars>
requires (std::integral<Ret> && 
  (std::same_as<char, Chars> && ...) && 
  (sizeof...(Chars) == sizeof(Ret)))
constexpr Ret MAGIC(Chars... chars) {
  const size_t count = sizeof...(chars);
  char str[count] = { chars... };

  Ret res{};

  for (size_t i = count - 1; i < count; --i) {
    size_t lsh = i << 3;
    res |= static_cast<Ret>(str[i]) << lsh;
  }

  return res;
}
