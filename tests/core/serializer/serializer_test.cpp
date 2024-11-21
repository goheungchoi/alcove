#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>

#include "core/serializer/serializer.h"

#include "nlohmann/json.hpp"

using json = nlohmann::json;

#include <string>
#include <iostream>

class TestSerializer : public JSONSerializer {
public:
    TestSerializer() : JSONSerializer() {}
};

TEST_CASE("Serializer: Set and Get Simple Data", "[serializer]") {
    TestSerializer serializer;

    SECTION("Set single key-value pairs") {
        serializer.SetData("name", "John");
        serializer.SetData("age", 30);
        serializer.SetData("is_active", true);

        const char* result = serializer.GetJSONString();
        REQUIRE(result != nullptr);

        std::string expected = json::parse(R"({"name":"John","age":30,"is_active":true})").dump();

        REQUIRE(std::string(result) == expected);
    }
}

TEST_CASE("Serializer: Handle Nested Keys", "[serializer]") {
    TestSerializer serializer;

    SECTION("Set nested key-value pairs") {
        serializer.SetData("name.first", "John");
        serializer.SetData("name.last", "Doe");
        serializer.SetData("address.city", "New York");
        serializer.SetData("address.zip", 10001);

        const char* result = serializer.GetJSONString();
        REQUIRE(result != nullptr);

        std::string expected = json::parse(R"({"name":{"first":"John","last":"Doe"},"address":{"city":"New York","zip":10001}})").dump();
        REQUIRE(std::string(result) == expected);
    }
}

TEST_CASE("Serializer: Handle Arrays", "[serializer]") {
    TestSerializer serializer;

    SECTION("Set array with initializer list") {
        serializer.SetArray("numbers", {1, 2, 3, 4, 5});
        serializer.SetArray("names", {"Alice", "Bob", "Charlie"});

        const char* result = serializer.GetJSONString();
        REQUIRE(result != nullptr);

        std::string expected = json::parse(R"({"numbers":[1,2,3,4,5],"names":["Alice","Bob","Charlie"]})").dump();
        REQUIRE(std::string(result) == expected);
    }
}

TEST_CASE("Serializer: Mixed Nested Data", "[serializer]") {
    TestSerializer serializer;

    SECTION("Set complex nested structures") {
        serializer.SetData("user.name.first", "John");
        serializer.SetData("user.name.last", "Doe");
        serializer.SetData("user.age", 30);
        serializer.SetArray("user.tags", {"admin", "editor", "user"});
        serializer.SetArray("user.occupancy", {"user", 2, "my"});

        serializer.SetData("user.address.city", "New York");
        serializer.SetData("user.address.zip", 10001);
        
        serializer.SetData("user.is_active", true);

        const char* result = serializer.GetJSONString();
        REQUIRE(result != nullptr);

        std::string expected = json::parse(R"({"user":{"name":{"first":"John","last":"Doe"},"age":30,"tags":["admin","editor","user"],"occupancy":["user",2,"my"],"address":{"city":"New York","zip":10001},"is_active":true}})").dump();
        REQUIRE(std::string(result) == expected);
    }
}

TEST_CASE("Serializer: Edge Cases", "[serializer]") {
    TestSerializer serializer;

    // SECTION("Empty keys and values") {
    //     serializer.SetData("", "empty_key");
    //     serializer.SetData("key", "");
    //     serializer.SetData("", "");

    //     const char* result = serializer.GetJSONString();
    //     REQUIRE(result != nullptr);

    //     std::string expected = json::parse(R"({"":"empty_key","key":""})").dump();
    //     REQUIRE(std::string(result) == R"({"":"empty_key","key":""})");
    // }

    SECTION("Null values") {
        serializer.SetData("key_with_null", nullptr);

        const char* result = serializer.GetJSONString();
        REQUIRE(result != nullptr);

        std::string expected = json::parse(R"({"key_with_null":null})").dump();
        REQUIRE(std::string(result) == R"({"key_with_null":null})");
    }

    SECTION("Overwrite existing keys") {
        serializer.SetData("key", "initial_value");
        serializer.SetData("key", "updated_value");

        const char* result = serializer.GetJSONString();
        REQUIRE(result != nullptr);

        std::string expected = json::parse(R"({"key":"updated_value"})").dump();
        REQUIRE(std::string(result) == R"({"key":"updated_value"})");
    }
}