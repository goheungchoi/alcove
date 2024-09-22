#include <core/engine/vk_descriptors.h>

void DescriptorLayoutBuilder::add_binding(
  uint32_t binding,
  VkDescriptorType type
) {
  VkDescriptorSetLayoutBinding bind {
    .binding = binding,
    .descriptorType = type,
    .descriptorCount = 1,
  };

  bindings.push_back(bind);
}

void DescriptorLayoutBuilder::clear() {
  bindings.clear();
}

VkDescriptorSetLayout DescriptorLayoutBuilder::build(
  VkDevice device,
  VkShaderStageFlags shaderStages,
  void* pNext,
  VkDescriptorSetLayoutCreateFlags flags
) {
  // Set the stage flags of the layout bindings
  for (auto& b : bindings) {
    b.stageFlags |= shaderStages;
  }

  VkDescriptorSetLayoutCreateInfo info {
    .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
    .pNext = pNext,

    .flags = flags,

    .bindingCount = static_cast<uint32_t>(bindings.size()),
    .pBindings = bindings.data()
  };

  VkDescriptorSetLayout inDescriptorSetLayout;
  VK_CHECK(
    vkCreateDescriptorSetLayout(
      device, 
      &info, 
      nullptr, 
      &inDescriptorSetLayout
    )
  );

  return inDescriptorSetLayout;
}

/**
 * @brief Initialize a descriptor pool.
 * @param device VkDevice
 * @param maxSets The max number of VkDescriptorSets that can be created from this allocator
 * @param poolRatios The array of PoolSizeRatio
 */
void DescriptorAllocator::init_pool(
  VkDevice device, 
  uint32_t maxSets, 
  std::span<PoolSizeRatio> poolRatios
) {
  std::vector<VkDescriptorPoolSize> poolSizes;

  // Check if the sum of descriptorCounts is less than or equal to maxSets.
  uint32_t totalDescriptorCount = 0U;
  for (PoolSizeRatio ratio : poolRatios) {
    uint32_t descriptorCount = static_cast<uint32_t>(ratio.ratio * maxSets);
    poolSizes.push_back(
      VkDescriptorPoolSize{
        .type = ratio.type,
        .descriptorCount = descriptorCount
      }
    );
    totalDescriptorCount += descriptorCount;
  }
  
// DEBUG: Error Check
#ifndef NDEBUG
  if (totalDescriptorCount > maxSets) {
    throw std::invalid_argument("DescriptorAllocator: Pool ratios are invalid!");
  }
#endif

  VkDescriptorPoolCreateInfo poolInfo {
    .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
    .flags = 0,
    .maxSets = maxSets,
    .poolSizeCount = static_cast<uint32_t>(poolSizes.size()),
    .pPoolSizes = poolSizes.data()
  };

  vkCreateDescriptorPool(device, &poolInfo, nullptr, &pool);
}

void DescriptorAllocator::clear_descriptors(VkDevice device) {
  vkResetDescriptorPool(device, pool, 0);
}

void DescriptorAllocator::destroy_pool(VkDevice device) {
  vkDestroyDescriptorPool(device, pool, nullptr);
}

VkDescriptorSet DescriptorAllocator::allocate(
  VkDevice device, 
  VkDescriptorSetLayout layout
) {
  VkDescriptorSetAllocateInfo descAllocInfo {
    .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
    .pNext = nullptr,
    .descriptorPool = pool,
    .descriptorSetCount = 1,
    .pSetLayouts = &layout
  };

  VkDescriptorSet descSet;
  VK_CHECK(
    vkAllocateDescriptorSets(
      device, 
      &descAllocInfo, 
      &descSet
    )
  );

  return descSet;
}

void GrowableDescriptorAllocator::init_pool(
  VkDevice device, 
  uint32_t initialSets,
  std::span<PoolSizeRatio> poolRatios) {

}
void GrowableDescriptorAllocator::clear_descriptors(VkDevice device) {

}
void GrowableDescriptorAllocator::destroy_pool(VkDevice device) {

}

VkDescriptorSet GrowableDescriptorAllocator::allocate(
  VkDevice device, 
  VkDescriptorSetLayout layout) {

}

VkDescriptorPool GrowableDescriptorAllocator::get_pool(VkDevice device) {
  VkDescriptorPool availablePool;
  if (readyPools.size() != 0) {
    availablePool = readyPools.back();
    readyPools.pop_back();
  } else {
    // Create a new pool
    availablePool = create_pool(device, setsPerPool, ratios);

    setsPerPool = setsPerPool * 1.5;
    if (setsPerPool > 4092) {
      setsPerPool = 4092;
    } 
  }

  return availablePool;
}
VkDescriptorPool GrowableDescriptorAllocator::create_pool(
  VkDevice device, 
  uint32_t setCount, 
  std::span<PoolSizeRatio> poolRatios) {
  std::vector<VkDescriptorPoolSize> poolSizes;
  for (PoolSizeRatio ratio : poolRatios) {
    VkDescriptorPoolSize poolSize {
      .type = ratio.type,
      .descriptorCount = uint32_t(ratio.ratio * setCount)
    };
    poolSizes.push_back(poolSize);
  }

  VkDescriptorPoolCreateInfo poolInfo {
    .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
    .pNext = nullptr,

    .flags = 0,
    .maxSets = setCount,
    .poolSizeCount = (uint32_t)poolSizes.size(),
    .pPoolSizes = poolSizes.data()
  };

  VkDescriptorPool newPool;
  vkCreateDescriptorPool(device, &poolInfo, nullptr, &newPool);
  return newPool;
}


