// shader_compiler_test.cpp
#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>

#include <filesystem>
#include <iostream>

#include "core/engine/compiler/shader_compiler.h"

class GLSLCompilerTest {
private:
  GLSLCompiler compiler;
public:
  GLSLCompilerTest(const std::string filename, bool output = false) : compiler(filename, output) {}

  const std::vector<unsigned int>& TestGetSpirvCode() const {
    return compiler.getSpirvCode();
  }
  const GLSLCompiler::NamesTuple TestGetStateNameAndBinaryNameFrom(const std::string& filename) const {
    return compiler.getStateNameAndBinaryNameFrom(filename);
  }
  const std::string TestReadShaderFile(const std::string filename) const {
    return compiler.readShaderFile(filename);
  }
};

TEST_CASE("Shader Compilation Test", "[GLSLCompiler]") {
  std::filesystem::path cwd = std::filesystem::current_path() / "filename.txt";
  std::cout << "The current working directory is " << cwd.string() << std::endl;

  const std::string filename = "shaders/shader.vert";
  GLSLCompilerTest comp_test(filename);
  REQUIRE(!comp_test.TestGetSpirvCode().empty());

  auto [state, bin] = comp_test.TestGetStateNameAndBinaryNameFrom(filename);
  REQUIRE(state == EShLangVertex);
  REQUIRE(bin == "vert.spv");

  std::string shader = comp_test.TestReadShaderFile(filename);
  std::string origin_shader = R"(#version 450

layout(location = 0) out vec3 fragColor;

vec2 positions[3] = vec2[](
    vec2(0.0, -0.5),
    vec2(0.5, 0.5),
    vec2(-0.5, 0.5)
);

vec3 colors[3] = vec3[](
    vec3(1.0, 0.0, 0.0),
    vec3(0.0, 1.0, 0.0),
    vec3(0.0, 0.0, 1.0)
);

void main() {
    gl_Position = vec4(positions[gl_VertexID], 0.0, 1.0);
    fragColor = colors[gl_VertexID];
})";
  REQUIRE(shader == origin_shader);
}