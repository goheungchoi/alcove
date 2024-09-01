# Compile the shader while configuring CMake files.
# It first finds `glslangValidator` from the VULKAN_SDK directory.
# Then, each shader file in the directory passed via the `path` parameter
# is collected and compiled with `glslangValidator`.
function(compile_glsl_shaders path) 

  # Check if glslangValidator exists
  if (NOT Vulkan_glslangValidator_FOUND)
    message(FATAL_ERROR "Vulkan shader compiler(glslangValidator) is not found!")
  endif()

  # Set the glslangValidator executable file hint path
  if(WIN32) 
    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
      set(_glsl_validator_hint_executable_search_paths
        "$ENV{VULKAN_SDK}/bin"
      )
    else()
      set(_glsl_validator_hint_executable_search_paths
        "$ENV{VULKAN_SDK}/bin32"
      )
    endif()
  else()
    set(_glsl_validator_hint_executable_search_paths
      "$ENV{VULKAN_SDK}/bin"
    )
  endif()

  # Find the glslang validator compiler
  find_program(GLSL_VALIDATOR
    glslangValidator
    HINTS
    _glsl_validator_hint_executable_search_paths
  )

  # If not found, raises an error
  if (GLSL_VALIDATOR-NOTFOUND)  
    message(FATAL_ERROR "glslangValidator is not found!")
  endif()

  # Find the shader files in the directory
  file(GLOB_RECURSE GLSL_SHADER_FILES
    "${path}/*.vert"
    "${path}/*.geom"
    "${path}/*.tesc"
    "${path}/*.tese"
    "${path}/*.frag"
    "${path}/*.comp"
    "${path}/*.mesh"
    "${path}/*.task"

    "${path}/*.glsl"
  )

  # Loop through all the shader files in the directory
  message(STATUS "\nSetting up shader compile targets...")
  foreach(GLSL ${GLSL_SHADER_FILES})
    message(STATUS "")

    # Set spir-v output file name of the current shader
    get_filename_component(FILE_NAME ${GLSL} NAME)
    set(SPIRV "${path}/${FILE_NAME}.spv")

    # Show messages
    message(STATUS ${GLSL})
    message(STATUS COMMAND "${GLSL_VALIDATOR} -V ${GLSL} -o ${SPIRV}")

    # Add a command to compile the shader files.
    add_custom_command(
      OUTPUT ${SPIRV}
      COMMAND ${GLSL_VALIDATOR} -V ${GLSL} -o ${SPIRV}
      DEPENDS ${GLSL}
    ) 

    # Append the output spir-v file to the list
    list(APPEND SPIRV_BINARY_FILES ${SPIRV})
  endforeach()

  add_custom_target(  # Create a target
    CompileShader
    ALL
    DEPENDS ${SPIRV_BINARY_FILES}
  )

  message(STATUS "\nCompile Shader target's ready!")
  message(STATUS "")

  unset(_glsl_validator_hint_executable_search_paths)
endfunction()