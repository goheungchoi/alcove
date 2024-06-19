// shader_compiler.cpp
#include "core/engine/compiler/shader_compiler.h"

#include <glslang/Public/ResourceLimits.h>
#include <glslang/SPIRV/GlslangToSpv.h>

#include <sstream>
#include <fstream>
#include <tuple>

/**
 * @brief Construct a new GLSLCompiler::GLSLCompiler object 
 * and initialize the `glslang` process.
 * 
 * @param filename 
 */
GLSLCompiler::GLSLCompiler(const std::string filename, bool output) {
  glslang::InitializeProcess();
  compileShader(filename, output);
}
/**
 * @brief Destroy the GLSLCompiler::GLSLCompiler object 
 * and finalize the `glslang` process.
 * 
 */
GLSLCompiler::~GLSLCompiler() {
  glslang::FinalizeProcess();
}
/**
 * @brief 
 * 
 * @param filename 
 */
void GLSLCompiler::compileShader(const std::string& filename, bool output) {
  // Read the shader file and convert it into std::string
  if (std::string shader_source = readShaderFile(filename); !shader_source.empty()) {
    const auto [stage_name, bin_name] = getStateNameAndBinaryNameFrom(filename);

    /* Start Compiling Process */ 
    const TBuiltInResource* resource = GetDefaultResources();
    const EShMessages messages = static_cast<EShMessages>(EShMsgSpvRules | EShMsgVulkanRules);

    // Shader processing
    glslang::TShader shader(stage_name);
    std::vector strings{shader_source.c_str()};
    shader.setStrings(strings.data(), strings.size());
    shader.setEnvInput(source_lang, stage_name, client, 100);
    shader.setEnvClient(client, client_version);
    shader.setEnvTarget(target_lang, target_lang_version);

    if ( !shader.parse(resource, 100, false, messages) ) {
      std::cerr << "GLSL parsing failed for: " << filename << std::endl;
      std::cerr << shader.getInfoLog() << std::endl;
      std::cerr << shader.getInfoDebugLog() << std::endl;
      return; /* TODO: Requires a more elagant way of handling errors */
    }

    // Program processing
    glslang::TProgram program;
    program.addShader(&shader);
    if ( !program.link(messages) ) {
      std::cerr << "Program linking failed for: " << filename << std::endl;
      std::cerr << program.getInfoLog() << std::endl;
      std::cerr << program.getInfoDebugLog() << std::endl;
      return; /* TODO: Requires a more elagant way of handling errors */
    }

    // Retrieve SPIR-V
    glslang::TIntermediate* intermediate = program.getIntermediate(stage_name);
    spv::SpvBuildLogger logger;
    glslang::GlslangToSpv(*intermediate, spir_v, &logger);

    if ( output && !glslang::OutputSpvBin(spir_v, bin_name.c_str()) ) {
      std::cerr << "Creating SPV binary file failed for: " << filename << std::endl;
      return; /* TODO: Requires a more elagant way of handling errors */
    }

    // TProgram must be deleted prior to freeing up TShaders
    delete &program;
  } else {
    std::cerr << "The shader resource is empty!" << std::endl;
    return; /* TODO: Requires a more elagant way of handling errors */
  }
}

/**
 * @brief Retrieves the state name from the extension name of the shader file.
 * 
 * @param filename The shader file name to get the state name.
 * @return `GLSLCompiler::NamesTuple` A tuple that contains `EShLanguage` and `std::string`.
 * It is equal to `std::tuple<EShLanguage, std::string>`.
 */
const GLSLCompiler::NamesTuple GLSLCompiler::getStateNameAndBinaryNameFrom(const std::string& filename) const {
  size_t extension_index = filename.find_last_of(".");
  std::string extension = filename.substr(extension_index);
  EShLanguage lang = EShLangVertex;
  // Extension names from glslang StandAlone app.
  if (extension == "vert")
    lang = EShLangVertex;
  else if (extension == "tesc")
    lang = EShLangTessControl;
  else if (extension == "tese")
    lang = EShLangTessEvaluation;
  else if (extension == "geom")
    lang = EShLangGeometry;
  else if (extension == "frag")
    lang = EShLangFragment;
  else if (extension == "comp")
    lang = EShLangCompute;
  else if (extension == "rgen")
    lang = EShLangRayGen;
  else if (extension == "rint")
    lang = EShLangIntersect;
  else if (extension == "rahit")
    lang = EShLangAnyHit;
  else if (extension == "rchit")
    lang = EShLangClosestHit;
  else if (extension == "rmiss")
    lang = EShLangMiss;
  else if (extension == "rcall")
    lang = EShLangCallable;
  else if (extension == "mesh")
    lang = EShLangMesh;
  else if (extension == "task")
    lang = EShLangTask;
  else
    std::cerr << "Stage name not found: " << extension << std::endl;
  return {lang, extension + ".spv"};
}
/**
 * @brief Reads a shader file and converts it to a string.
 *
 * @param filename The shader file name to be converted
 * @return `std::string` The context of the shader file in `std::string` format
 */
const std::string GLSLCompiler::readShaderFile(const std::string& filename) const {
  std::ifstream ifile(filename);

  if (!ifile.is_open()) {
    std::cerr << "Failed to open file: " << filename << std::endl;
    return std::string{};
  }

  // basic_ostream& operator<<( std::basic_streambuf<CharT, Traits>* sb ); (17)
  //   17) This function behaves as an UnformattedOutputFunction.
  // After constructing and checking the sentry object, checks if sb is a null pointer.
  // If it is, executes setstate(badbit) and exits.
  // Otherwise, extracts characters from the input sequence controlled by sb
  // and inserts them into *this until one of the following conditions are met:
  //  - end-of-file occurs on the input sequence;
  //  - inserting in the output sequence fails (in which case the character to be inserted is not extracted);
  //  - an exception occurs (in which case the exception is caught).
  // If no characters were inserted, executes setstate(failbit).
  // If an exception was thrown while extracting, sets failbit and,
  // if failbit is set in exceptions(), rethrows the exception.
  std::stringstream buffer;
  buffer << ifile.rdbuf();
  if (!buffer.good()) {
    std::cerr << "Error occurred while reading from buffer: " << filename << std::endl;
    return std::string{};
  }

  return buffer.str();
}
