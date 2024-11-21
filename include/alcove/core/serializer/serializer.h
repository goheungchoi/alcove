#pragma once

#include <utility>
#include <tuple>
#include <type_traits>

template<typename V>
concept IsValidJSONValueType = 
  std::is_null_pointer_v<V> ||
  std::is_integral_v<V> ||
  std::is_floating_point_v<V> ||
  std::is_convertible_v<V, const char*>;

class Serializer {
  struct Private;
  Private* _m;

public:

  Serializer();
  virtual ~Serializer();

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

  template<typename T>
  requires ( IsValidJSONValueType<T> )
  void SetArray(const char* nestedKey, std::initializer_list<T> ilist) {
    SetArray(nestedKey);

    auto it = ilist.begin();
    for (const T v : ilist) {
      AppendData(nestedKey, v);
    }
  }

  template<typename... Args>
  requires (IsValidJSONValueType<Args> && ...)
  void SetArray(const char* nestedKey, std::tuple<Args...>&& t) {
    SetArray(nestedKey);

    std::apply([this, nestedKey] (auto&&... args) {
      (((void) AppendData(nestedKey, args)), ...);
    }, std::move(t));
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


  // void SetObject(const char* nestedKey);
  // void SetObject(const char* nestedKey, const std::pair<const char*, nullptr_t>&& obj);
  // void SetObject(const char* nestedKey, const std::pair<const char*, bool>&& obj);
  // void SetObject(const char* nestedKey, const std::pair<const char*, char>&& obj);
  // void SetObject(const char* nestedKey, const std::pair<const char*, short>&& obj);
  // void SetObject(const char* nestedKey, const std::pair<const char*, int>&& obj);
  // void SetObject(const char* nestedKey, const std::pair<const char*, long>&& obj);
  // void SetObject(const char* nestedKey, const std::pair<const char*, long long>&& obj);
  // void SetObject(const char* nestedKey, const std::pair<const char*, unsigned char>&& obj);
  // void SetObject(const char* nestedKey, const std::pair<const char*, unsigned short>&& obj);
  // void SetObject(const char* nestedKey, const std::pair<const char*, unsigned int>&& obj);
  // void SetObject(const char* nestedKey, const std::pair<const char*, unsigned long>&& obj);
  // void SetObject(const char* nestedKey, const std::pair<const char*, unsigned long long>&& obj);
  // void SetObject(const char* nestedKey, const std::pair<const char*, float>&& obj);
  // void SetObject(const char* nestedKey, const std::pair<const char*, double>&& obj);
  // void SetObject(const char* nestedKey, const std::pair<const char*, const char*>&& obj);
  
  // template<typename... Args>
  // requires (IsValidJSONValueType<Args> && ...)
  // void SetObject(const char* nestedKey, std::tuple<std::pair<const char*, Args>...>&& t) {
  //   SetObject(nestedKey);

  //   std::apply([this, nestedKey] (auto&&... args) {
  //     ( ((void) AppendObject(nestedKey, std::move(args))), ... );
  //   }, std::move(t));
  // }

  // void AppendObject(const char* nestedKey, const std::pair<const char*, nullptr_t>& obj);
  // void AppendObject(const char* nestedKey, const std::pair<const char*, bool>& obj);
  // void AppendObject(const char* nestedKey, const std::pair<const char*, char>& obj);
  // void AppendObject(const char* nestedKey, const std::pair<const char*, short>& obj);
  // void AppendObject(const char* nestedKey, const std::pair<const char*, int>& obj);
  // void AppendObject(const char* nestedKey, const std::pair<const char*, long>& obj);
  // void AppendObject(const char* nestedKey, const std::pair<const char*, long long>& obj);
  // void AppendObject(const char* nestedKey, const std::pair<const char*, unsigned char>& obj);
  // void AppendObject(const char* nestedKey, const std::pair<const char*, unsigned short>& obj);
  // void AppendObject(const char* nestedKey, const std::pair<const char*, unsigned int>& obj);
  // void AppendObject(const char* nestedKey, const std::pair<const char*, unsigned long>& obj);
  // void AppendObject(const char* nestedKey, const std::pair<const char*, unsigned long long>& obj);
  // void AppendObject(const char* nestedKey, const std::pair<const char*, float>& obj);
  // void AppendObject(const char* nestedKey, const std::pair<const char*, double>& obj);
  // void AppendObject(const char* nestedKey, const std::pair<const char*, const char*>& obj);

  const char* GetJSONString();

  void Serialize(const char* dir, const char* outfilename);
};
