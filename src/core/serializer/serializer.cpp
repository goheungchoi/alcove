#include "core/serializer/serializer.h"

#include <nlohmann/json.hpp>
#include <sstream>
#include <string>
#include <fstream>

using json = nlohmann::json;

struct Serializer::Private {
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

Serializer::Serializer() { _m = new Private; }

Serializer::~Serializer() { delete _m; }

void Serializer::Parse(const char* str) { _m->j = json::parse(str); }

void Serializer::SetData(const char* nestedKey, nullptr_t value) {
  _m->get_nested_key(nestedKey) = value;
}
void Serializer::SetData(const char* nestedKey, bool value) {
  _m->get_nested_key(nestedKey) = value;
}
void Serializer::SetData(const char* nestedKey, const char value) {
  _m->get_nested_key(nestedKey) = value;
}
void Serializer::SetData(const char* nestedKey, const short value) {
  _m->get_nested_key(nestedKey) = value;
}
void Serializer::SetData(const char* nestedKey, const int value) {
  _m->get_nested_key(nestedKey) = value;
}
void Serializer::SetData(const char* nestedKey, const long value) {
  _m->get_nested_key(nestedKey) = value;
}
void Serializer::SetData(const char* nestedKey, const long long value) {
  _m->get_nested_key(nestedKey) = value;
}
void Serializer::SetData(const char* nestedKey, const unsigned char value) {
  _m->get_nested_key(nestedKey) = value;
}
void Serializer::SetData(const char* nestedKey, const unsigned short value) {
  _m->get_nested_key(nestedKey) = value;
}
void Serializer::SetData(const char* nestedKey, const unsigned int value) {
  _m->get_nested_key(nestedKey) = value;
}
void Serializer::SetData(const char* nestedKey, const unsigned long value) {
  _m->get_nested_key(nestedKey) = value;
}
void Serializer::SetData(const char* nestedKey, const unsigned long long value) {
  _m->get_nested_key(nestedKey) = value;
}
void Serializer::SetData(const char* nestedKey, const float value) {
  _m->get_nested_key(nestedKey) = value;
}
void Serializer::SetData(const char* nestedKey, const double value) {
  _m->get_nested_key(nestedKey) = value;
}
void Serializer::SetData(const char* nestedKey, const char* value) {
  _m->get_nested_key(nestedKey) = value;
}

void Serializer::SetArray(const char* nestedKey) {
  _m->get_nested_key(nestedKey) = json::value_type::array();
}
void Serializer::AppendData(const char* nestedKey, nullptr_t value) {
  _m->get_nested_key(nestedKey).push_back(value);
}
void Serializer::AppendData(const char* nestedKey, bool value) {
  _m->get_nested_key(nestedKey).push_back(value);
}
void Serializer::AppendData(const char* nestedKey, const char value) {
  _m->get_nested_key(nestedKey).push_back(value);
}
void Serializer::AppendData(const char* nestedKey, const short value) {
  _m->get_nested_key(nestedKey).push_back(value);
}
void Serializer::AppendData(const char* nestedKey, const int value) {
  _m->get_nested_key(nestedKey).push_back(value);
}
void Serializer::AppendData(const char* nestedKey, const long value) {
  _m->get_nested_key(nestedKey).push_back(value);
}
void Serializer::AppendData(const char* nestedKey, const long long value) {
  _m->get_nested_key(nestedKey).push_back(value);
}
void Serializer::AppendData(const char* nestedKey, const unsigned char value) {
  _m->get_nested_key(nestedKey).push_back(value);
}
void Serializer::AppendData(const char* nestedKey, const unsigned short value) {
  _m->get_nested_key(nestedKey).push_back(value);
}
void Serializer::AppendData(const char* nestedKey, const unsigned int value) {
  _m->get_nested_key(nestedKey).push_back(value);
}
void Serializer::AppendData(const char* nestedKey, const unsigned long value) {
  _m->get_nested_key(nestedKey).push_back(value);
}
void Serializer::AppendData(const char* nestedKey, const unsigned long long value) {
  _m->get_nested_key(nestedKey).push_back(value);
}
void Serializer::AppendData(const char* nestedKey, const float value) {
  _m->get_nested_key(nestedKey).push_back(value);
}
void Serializer::AppendData(const char* nestedKey, const double value) {
  _m->get_nested_key(nestedKey).push_back(value);
}
void Serializer::AppendData(const char* nestedKey, const char* value) {
  _m->get_nested_key(nestedKey).push_back(value);
}

// void Serializer::SetObject(const char* nestedKey) {
//   _m->get_nested_key(nestedKey) = json::value_type::object();
// }
// void Serializer::SetObject(const char* nestedKey,
//                            const std::pair<const char*, nullptr_t>&& obj) {
//   _m->get_nested_key(nestedKey) = {obj};
// }
// void Serializer::SetObject(const char* nestedKey,
//                            const std::pair<const char*, bool>&& obj) {
//   _m->get_nested_key(nestedKey) = {obj};
// }
// void Serializer::SetObject(const char* nestedKey,
//                            const std::pair<const char*, char>&& obj) {
//   _m->get_nested_key(nestedKey) = {obj};
// }
// void Serializer::SetObject(const char* nestedKey,
//                            const std::pair<const char*, short>&& obj) {
//   _m->get_nested_key(nestedKey) = {obj};
// }
// void Serializer::SetObject(const char* nestedKey, const std::pair<const char*, int>&& obj) {
//   _m->get_nested_key(nestedKey) = {obj};
// }
// void Serializer::SetObject(const char* nestedKey,
//                            const std::pair<const char*, long>&& obj) {
//   _m->get_nested_key(nestedKey) = {obj};
// }
// void Serializer::SetObject(const char* nestedKey,
//                            const std::pair<const char*, long long>&& obj) {
//   _m->get_nested_key(nestedKey) = {obj};
// }
// void Serializer::SetObject(const char* nestedKey,
//                            const std::pair<const char*, unsigned char>&& obj) {
//   _m->get_nested_key(nestedKey) = {obj};
// }
// void Serializer::SetObject(const char* nestedKey,
//                            const std::pair<const char*, unsigned short>&& obj) {
//   _m->get_nested_key(nestedKey) = {obj};
// }
// void Serializer::SetObject(const char* nestedKey,
//                            const std::pair<const char*, unsigned int>&& obj) {
//   _m->get_nested_key(nestedKey) = {obj};
// }
// void Serializer::SetObject(const char* nestedKey,
//                            const std::pair<const char*, unsigned long>&& obj) {
//   _m->get_nested_key(nestedKey) = {obj};
// }
// void Serializer::SetObject(const char* nestedKey,
//                            const std::pair<const char*, unsigned long long>&& obj) {
//   _m->get_nested_key(nestedKey) = {obj};
// }
// void Serializer::SetObject(const char* nestedKey,
//                            const std::pair<const char*, float>&& obj) {
//   _m->get_nested_key(nestedKey) = {obj};
// }
// void Serializer::SetObject(const char* nestedKey,
//                            const std::pair<const char*, double>&& obj) {
//   _m->get_nested_key(nestedKey) = {obj};
// }
// void Serializer::SetObject(const char* nestedKey,
//                            const std::pair<const char*, const char*>&& obj) {
//   _m->get_nested_key(nestedKey) = {obj};
// }

// void Serializer::AppendObject(const char* nestedKey,
//                               const std::pair<const char*, nullptr_t>& obj) {
//   _m->get_nested_key(nestedKey).push_back(json::value_type::object({std::move(obj)}));
// }
// void Serializer::AppendObject(const char* nestedKey,
//                               const std::pair<const char*, bool>& obj) {
//   _m->get_nested_key(nestedKey).push_back(json::value_type::object({std::move(obj)}));
// }
// void Serializer::AppendObject(const char* nestedKey,
//                               const std::pair<const char*, char>& obj) {
//   _m->get_nested_key(nestedKey).push_back(json::value_type::object({std::move(obj)}));
// }
// void Serializer::AppendObject(const char* nestedKey,
//                               const std::pair<const char*, short>& obj) {
//   _m->get_nested_key(nestedKey).push_back(json::value_type::object({std::move(obj)}));
// }
// void Serializer::AppendObject(const char* nestedKey,
//                               const std::pair<const char*, int>& obj) {
//   _m->get_nested_key(nestedKey).push_back(json::value_type::object({std::move(obj)}));
// }
// void Serializer::AppendObject(const char* nestedKey,
//                               const std::pair<const char*, long>& obj) {
//   _m->get_nested_key(nestedKey).push_back(json::value_type::object({std::move(obj)}));
// }
// void Serializer::AppendObject(const char* nestedKey,
//                               const std::pair<const char*, long long>& obj) {
//   _m->get_nested_key(nestedKey).push_back(json::value_type::object({std::move(obj)}));
// }
// void Serializer::AppendObject(const char* nestedKey,
//                               const std::pair<const char*, unsigned char>& obj) {
//   _m->get_nested_key(nestedKey).push_back(json::value_type::object({std::move(obj)}));
// }
// void Serializer::AppendObject(const char* nestedKey,
//                               const std::pair<const char*, unsigned short>& obj) {
//   _m->get_nested_key(nestedKey).push_back(json::value_type::object({std::move(obj)}));
// }
// void Serializer::AppendObject(const char* nestedKey,
//                               const std::pair<const char*, unsigned int>& obj) {
//   _m->get_nested_key(nestedKey).push_back(json::value_type::object({std::move(obj)}));
// }
// void Serializer::AppendObject(const char* nestedKey,
//                               const std::pair<const char*, unsigned long>& obj) {
//   _m->get_nested_key(nestedKey).push_back(json::value_type::object({std::move(obj)}));
// }
// void Serializer::AppendObject(
//     const char* nestedKey, const std::pair<const char*, unsigned long long>& obj) {
//   _m->get_nested_key(nestedKey).push_back(json::value_type::object({std::move(obj)}));
// }
// void Serializer::AppendObject(const char* nestedKey,
//                               const std::pair<const char*, float>& obj) {
//   _m->get_nested_key(nestedKey).push_back(json::value_type::object({std::move(obj)}));
// }
// void Serializer::AppendObject(const char* nestedKey,
//                               const std::pair<const char*, double>& obj) {
//   _m->get_nested_key(nestedKey).push_back(json::value_type::object({std::move(obj)}));
// }
// void Serializer::AppendObject(const char* nestedKey,
//                               const std::pair<const char*, const char*>& obj) {
//   _m->get_nested_key(nestedKey).push_back(json::value_type::object({std::move(obj)}));
// }

const char* Serializer::GetJSONString() { 
  static std::string jsonString;
  jsonString = _m->j.dump(); // Serialize JSON object to a string
  return jsonString.c_str();
}

void Serializer::Serialize(const char* dir, const char* outfilename) { 



}
