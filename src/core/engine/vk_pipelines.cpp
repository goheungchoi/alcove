#include <core/engine/vk_pipelines.h>

bool vkutil::load_shader_module(
  const char* filepath,
  VkDevice device,
  VkShaderModule* outShaderModule
)  {
  // Open the file with the cursor at the end
  std::ifstream file(filepath, std::ios::ate | std::ios::binary);

  if (!file.is_open()) {
    return false;
  }

  // The location of the cursor tells the size of 
  // the file in bytes
  std::size_t filesize = static_cast<std::size_t>(file.tellg());

  // spirv expects the buffer to be on uint32_t,
  // so make sure to reserve a int vector big enough
  // for the entire file
  std::vector<uint32_t> buffer(filesize / sizeof(uint32_t));

  // Place the fie cursor at the beginning
  file.seekg(0);

  // Load the entire file into the buffer
  file.read((char*)buffer.data(), filesize);

  // Now that the file is loaded into the buffer, we can close it
  file.close();

  // Create a new shader module, using the buffer we loaded
  VkShaderModuleCreateInfo shaderCreateInfo {
    .sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
    .pNext = nullptr,

    // Code size has to be in bytes,
    // so multiply the ints in the buffer by the size of int
    // to know the real size of the buffer
    .codeSize = buffer.size() * sizeof(uint32_t),
    .pCode = buffer.data()
  };

  // Check that the creation goes well
  VkShaderModule shaderModule;
  if (vkCreateShaderModule(device, &shaderCreateInfo, nullptr, &shaderModule) != VK_SUCCESS) {
    return false;
  }

  // Return the shader module handle
  *outShaderModule = shaderModule;
  return true;
}


