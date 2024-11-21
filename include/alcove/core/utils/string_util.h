#pragma once

#include <string>
#include <string_view>
#include <format>

namespace al {

template<typename... Args>
inline constexpr
  std::string text(Args&&... args) {
  return (std::string() + ... + std::format("{}", args));
}

template<typename... Args>
inline constexpr
  std::string format(std::string_view vstr, Args&&... args) {
  return std::vformat(vstr, std::make_format_args(args...));
}

template<typename... Args>
inline constexpr
  int fprint(std::FILE* stream, Args&&... args) {
  std::string str = text(std::forward<Args>(args)...);
  return std::fprintf(stream, "%s", str.c_str());
}

template<typename... Args>
inline constexpr
  int fprintf(std::FILE* stream, std::string_view vstr, Args&&... args) {
  std::string str = format(vstr, std::forward<Args>(args)...);
  return std::fprintf(stream, "%s", str.c_str());
}

template<typename... Args>
inline constexpr
  int fprintln(std::FILE* stream, Args&&... args) {
  std::string str = text(std::forward<Args>(args)...);
  return std::fprintf(stream, "%s\n", str.c_str());
}

template<typename... Args>
inline constexpr
  int console_out(Args&&... args) {
  std::string str = text(std::forward<Args>(args)...);
  return std::fprintf(stdout, "%s\n", str.c_str());
}

}
