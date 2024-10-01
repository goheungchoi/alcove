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
  // Assign pool size ratios
  ratios.assign(poolRatios.begin(), poolRatios.end());

  // Create the first pool
  VkDescriptorPool newPool = create_pool(device, initialSets, poolRatios);
  readyPools.push_back(newPool);

  // The pool size of the next allocation
  setsPerPool = initialSets * GROW_RATE; 

}
void GrowableDescriptorAllocator::clear_descriptors(VkDevice device) {
  // Reset the pools in the ready pool
  for (auto pool : readyPools) {
    // NOTE: Does reseting descriptor pool require the communication with GPU?
    // If so, how can I optimize it?
    vkResetDescriptorPool(device, pool, 0);
  }
  // Reset the pools in the full pools
  for (auto pool : fullPools) {
    vkResetDescriptorPool(device, pool, 0);
    // Move the pool to the ready pools
    readyPools.push_back(pool);
  }
  fullPools.clear();
}
void GrowableDescriptorAllocator::destroy_pool(VkDevice device) {
  // Clear readyPools
  for (auto pool : readyPools) {
    vkDestroyDescriptorPool(device, pool, nullptr);
  }
  readyPools.clear();
  // Clear fullPools
  for (auto pool : fullPools) {
    vkDestroyDescriptorPool(device, pool, nullptr);
  }
  fullPools.clear();
}

VkDescriptorSet GrowableDescriptorAllocator::allocate(
  VkDevice device, 
  VkDescriptorSetLayout layout,
  void* pNext) {
  // Get an available pool
  VkDescriptorPool poolToUse = get_pool(device);

  VkDescriptorSetAllocateInfo allocInfo{
    .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
    .pNext = pNext,

    .descriptorPool = poolToUse,
    .descriptorSetCount = 1,
    .pSetLayouts = &layout
  };

  VkDescriptorSet descriptorSet;
  VkResult res = vkAllocateDescriptorSets(device, &allocInfo, &descriptorSet);

  // If allocation failed either due to the shortage of the pool memory or
  // fragmentation of the pool's memory, try again with a new available pool.
  if (res == VK_ERROR_OUT_OF_POOL_MEMORY || res == VK_ERROR_FRAGMENTED_POOL) {
    fullPools.push_back(poolToUse);

    poolToUse = get_pool(device);
    allocInfo.descriptorPool = poolToUse;

    VK_CHECK(vkAllocateDescriptorSets(device, &allocInfo, &descriptorSet));
  }

  // Once we are done with allocation of a set with a pool, 
  // push it back into the ready pool.
  readyPools.push_back(poolToUse);

  return descriptorSet;
}

VkDescriptorPool GrowableDescriptorAllocator::get_pool(VkDevice device) {
  VkDescriptorPool availablePool;
  if (readyPools.size() != 0) {
    // Get an available pool from the ready pools
    availablePool = readyPools.back();
    // Pop back the ready pool
    readyPools.pop_back();
  } else {
    // Create a new pool
    availablePool = create_pool(device, setsPerPool, ratios);
    // Increase the number of sets per pool
    setsPerPool = setsPerPool * GROW_RATE;
    // Fix the max number of sets per pool
    if (setsPerPool > MAX_DESCRIPTOR_SETS_PER_POOL) {
      setsPerPool = MAX_DESCRIPTOR_SETS_PER_POOL;
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

void DescriptorWriter::write_image(
  int binding, 
  VkImageView image, 
  VkSampler sampler, 
  VkImageLayout layout, 
  VkDescriptorType type) {
  // Setup a descriptor image info and retrieve its reference 
  // from the image infos array
  VkDescriptorImageInfo& info = imageInfos.emplace_back(
    VkDescriptorImageInfo{
      .sampler = sampler,
      .imageView = image,
      .imageLayout = layout
    }
  );

  // Record the write descriptor set info of the descriptor image
  VkWriteDescriptorSet write{
    .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,

    .dstSet = VK_NULL_HANDLE, // Left empty for now until we need to write it
    .dstBinding = binding,

    .descriptorCount = 1,
    .descriptorType = type,
    .pImageInfo = &info
  };
  writes.push_back(write);
}


void DescriptorWriter::write_buffer(
  int binding, 
  VkBuffer buffer, 
  std::size_t size, 
  std::size_t offset, 
  VkDescriptorType type) {
  // Setup a descriptor buffer info and retrieve its reference 
  // from the buffer infos array
  VkDescriptorBufferInfo& info = bufferInfos.emplace_back(
    VkDescriptorBufferInfo{
      .buffer = buffer,
      .offset = offset,
      .range = size
    }
  );

  // Record the write descriptor set info of the descriptor buffer
  VkWriteDescriptorSet write{
    .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,

    .dstSet = VK_NULL_HANDLE, // Left empty for now until we need to write it
    .dstBinding = binding,

    .descriptorCount = 1,
    .descriptorType = type,
    .pBufferInfo = &info
  };
  writes.push_back(write);
}

void DescriptorWriter::clear() {
  imageInfos.clear();
  bufferInfos.clear();
  writes.clear();
}

void DescriptorWriter::update_set(VkDevice device, VkDescriptorSet descSet) {
  for (VkWriteDescriptorSet& write : writes) {
    write.dstSet = descSet;
  }

  vkUpdateDescriptorSets(device, (uint32_t)writes.size(), writes.data(), 0, nullptr);
}
