#include "core/serializer/serializer.h"

#include <nlohmann/json.hpp>
#include <sstream>
#include <string>
#include <fstream>

using json = nlohmann::json;

struct JSONSerializer::Private {
  json j{};

  json& get_nested_key(const char* nested_keys) {
    std::reference_wrapper<json> json_ref{j};

    std::string keys(nested_keys);
    std::istringstream ss(keys);

    std::string nestedKey;
    while (std::getline(ss, nestedKey, '.')) {
      json_ref = json_ref.get()[nestedKey];
    }

    return json_ref;
  };
};

JSONSerializer::JSONSerializer() { _m = new Private; }

JSONSerializer::~JSONSerializer() { delete _m; }

void JSONSerializer::Parse(const char* str) { _m->j = json::parse(str); }

void JSONSerializer::SetData(const char* nestedKey, nullptr_t value) {
  _m->get_nested_key(nestedKey) = value;
}
void JSONSerializer::SetData(const char* nestedKey, bool value) {
  _m->get_nested_key(nestedKey) = value;
}
void JSONSerializer::SetData(const char* nestedKey, const char value) {
  _m->get_nested_key(nestedKey) = value;
}
void JSONSerializer::SetData(const char* nestedKey, const short value) {
  _m->get_nested_key(nestedKey) = value;
}
void JSONSerializer::SetData(const char* nestedKey, const int value) {
  _m->get_nested_key(nestedKey) = value;
}
void JSONSerializer::SetData(const char* nestedKey, const long value) {
  _m->get_nested_key(nestedKey) = value;
}
void JSONSerializer::SetData(const char* nestedKey, const long long value) {
  _m->get_nested_key(nestedKey) = value;
}
void JSONSerializer::SetData(const char* nestedKey, const unsigned char value) {
  _m->get_nested_key(nestedKey) = value;
}
void JSONSerializer::SetData(const char* nestedKey, const unsigned short value) {
  _m->get_nested_key(nestedKey) = value;
}
void JSONSerializer::SetData(const char* nestedKey, const unsigned int value) {
  _m->get_nested_key(nestedKey) = value;
}
void JSONSerializer::SetData(const char* nestedKey, const unsigned long value) {
  _m->get_nested_key(nestedKey) = value;
}
void JSONSerializer::SetData(const char* nestedKey, const unsigned long long value) {
  _m->get_nested_key(nestedKey) = value;
}
void JSONSerializer::SetData(const char* nestedKey, const float value) {
  _m->get_nested_key(nestedKey) = value;
}
void JSONSerializer::SetData(const char* nestedKey, const double value) {
  _m->get_nested_key(nestedKey) = value;
}
void JSONSerializer::SetData(const char* nestedKey, const char* value) {
  _m->get_nested_key(nestedKey) = value;
}

void JSONSerializer::SetArray(const char* nestedKey) {
  _m->get_nested_key(nestedKey) = json::value_type::array();
}
void JSONSerializer::AppendData(const char* nestedKey, nullptr_t value) {
  _m->get_nested_key(nestedKey).push_back(value);
}
void JSONSerializer::AppendData(const char* nestedKey, bool value) {
  _m->get_nested_key(nestedKey).push_back(value);
}
void JSONSerializer::AppendData(const char* nestedKey, const char value) {
  _m->get_nested_key(nestedKey).push_back(value);
}
void JSONSerializer::AppendData(const char* nestedKey, const short value) {
  _m->get_nested_key(nestedKey).push_back(value);
}
void JSONSerializer::AppendData(const char* nestedKey, const int value) {
  _m->get_nested_key(nestedKey).push_back(value);
}
void JSONSerializer::AppendData(const char* nestedKey, const long value) {
  _m->get_nested_key(nestedKey).push_back(value);
}
void JSONSerializer::AppendData(const char* nestedKey, const long long value) {
  _m->get_nested_key(nestedKey).push_back(value);
}
void JSONSerializer::AppendData(const char* nestedKey, const unsigned char value) {
  _m->get_nested_key(nestedKey).push_back(value);
}
void JSONSerializer::AppendData(const char* nestedKey, const unsigned short value) {
  _m->get_nested_key(nestedKey).push_back(value);
}
void JSONSerializer::AppendData(const char* nestedKey, const unsigned int value) {
  _m->get_nested_key(nestedKey).push_back(value);
}
void JSONSerializer::AppendData(const char* nestedKey, const unsigned long value) {
  _m->get_nested_key(nestedKey).push_back(value);
}
void JSONSerializer::AppendData(const char* nestedKey, const unsigned long long value) {
  _m->get_nested_key(nestedKey).push_back(value);
}
void JSONSerializer::AppendData(const char* nestedKey, const float value) {
  _m->get_nested_key(nestedKey).push_back(value);
}
void JSONSerializer::AppendData(const char* nestedKey, const double value) {
  _m->get_nested_key(nestedKey).push_back(value);
}
void JSONSerializer::AppendData(const char* nestedKey, const char* value) {
  _m->get_nested_key(nestedKey).push_back(value);
}

const char* JSONSerializer::GetJSONString(const int indent, int* outSize) { 
  static std::string jsonString;
  jsonString = _m->j.dump(indent); // Serialize JSON object to a string
  if (outSize) *outSize = jsonString.size() + 1;
  return jsonString.c_str();
}
