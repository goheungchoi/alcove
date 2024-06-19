#pragma once

#include <core/engine/vk_common.h>

struct DescriptorLayoutBuilder {

  std::vector<VkDescriptorSetLayoutBinding> bindings;

  void add_binding(uint32_t binding, VkDescriptorType type);
  
  void clear();

  VkDescriptorSetLayout build(
    VkDevice device, 
    VkShaderStageFlags shaderStages,
    void* pNext = nullptr,
    VkDescriptorSetLayoutCreateFlags flags = 0
  );
};
