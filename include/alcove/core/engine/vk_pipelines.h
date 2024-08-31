#include <core/engine/vk_common.h>

#include <fstream>

namespace vkutil {

bool load_shader_module(
  const char* filepath,
  VkDevice device,
  VkShaderModule* outShaderModule
);

}
