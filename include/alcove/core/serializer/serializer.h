#pragma once

#include <variant>

class JSONSerializer {
  struct Private;
  Private* _m;

  using JSONValue = std::variant<nullptr_t, bool, char, short, int, long, long long, unsigned char,
    unsigned int, unsigned long, unsigned long long, float, double, const char*>;

public:

  JSONSerializer();
  virtual ~JSONSerializer();

  void Parse(const char* str);

  void SetData(const char* nestedKey, nullptr_t value);
  void SetData(const char* nestedKey, const bool value);
  void SetData(const char* nestedKey, const char value);
  void SetData(const char* nestedKey, const short value);
  void SetData(const char* nestedKey, const int value);
  void SetData(const char* nestedKey, const long value);
  void SetData(const char* nestedKey, const long long value);
  void SetData(const char* nestedKey, const unsigned char value);
  void SetData(const char* nestedKey, const unsigned short value);
  void SetData(const char* nestedKey, const unsigned int value);
  void SetData(const char* nestedKey, const unsigned long value);
  void SetData(const char* nestedKey, const unsigned long long value);
  void SetData(const char* nestedKey, const float value);
  void SetData(const char* nestedKey, const double value);
  void SetData(const char* nestedKey, const char* value);


  void SetArray(const char* nestedKey);

  void SetArray(const char* nestedKey, std::initializer_list<JSONValue>&& ilist) {
    SetArray(nestedKey);
    for (const auto& elem : ilist) {
      std::visit([this, nestedKey] (auto&& arg) {
        AppendData(nestedKey, arg);
      }, elem);
    }
  }

  void AppendData(const char* nestedKey, nullptr_t value);
  void AppendData(const char* nestedKey, const bool value);
  void AppendData(const char* nestedKey, const char value);
  void AppendData(const char* nestedKey, const short value);
  void AppendData(const char* nestedKey, const int value);
  void AppendData(const char* nestedKey, const long value);
  void AppendData(const char* nestedKey, const long long value);
  void AppendData(const char* nestedKey, const unsigned char value);
  void AppendData(const char* nestedKey, const unsigned short value);
  void AppendData(const char* nestedKey, const unsigned int value);
  void AppendData(const char* nestedKey, const unsigned long value);
  void AppendData(const char* nestedKey, const unsigned long long value);
  void AppendData(const char* nestedKey, const float value);
  void AppendData(const char* nestedKey, const double value);
  void AppendData(const char* nestedKey, const char* value);

  const char* GetJSONString();
};
