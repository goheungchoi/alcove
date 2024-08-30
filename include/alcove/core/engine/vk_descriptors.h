#pragma once

#include <core/engine/vk_common.h>

struct DescriptorLayoutBuilder {
  std::vector<VkDescriptorSetLayoutBinding> bindings;

  void add_binding(uint32_t binding, VkDescriptorType type);
  
  void clear();

  /**
   * @brief Build a descriptor set layout. Descriptor set layouts
   * specify which data will be bound to descriptor sets.
   * 
   * @param device 
   * @param shaderStages 
   * @param pNext 
   * @param flags 
   * @return VkDescriptorSetLayout 
   */
  VkDescriptorSetLayout build(
    VkDevice device, 
    VkShaderStageFlags shaderStages,
    void* pNext = nullptr,
    VkDescriptorSetLayoutCreateFlags flags = 0
  );
};

struct DescriptorAllocator {
  struct PoolSizeRatio {
    // The type of the descriptor that the allocator will contain
    VkDescriptorType type; 
    // The ratio of this type of descriptors in the pool
    // e.g., For an pool size ratio of uniform buffers,
    //  ```
    //  PoolSizeRatio uniformBufferRatio{
    //    .type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
    //    .ratio = 0.8
    //  };
    //  ```
    //  , if maxSets is 10, then, the descriptor allocator can
    // contain up to 8 uniform buffers.
    float ratio;
  };

  VkDescriptorPool pool;

  /**
   * @brief Initialize a descriptor pool.
   * 
   * @param device 
   * @param maxSets 
   * @param poolRatios 
   */
  void init_pool(
    VkDevice device, 
    uint32_t maxSets,
    std::span<PoolSizeRatio> poolRatios
  );
  void clear_descriptors(VkDevice device);
  void destroy_pool(VkDevice device);

  /**
   * @brief Allocate a descriptor set from the descriptor
   * pool in this allocator.
   * 
   * @param device 
   * @param layout The descriptor set layout that contains information about
   * what data the descriptor set holds.
   * @return VkDescriptorSet 
   */
  VkDescriptorSet allocate(
    VkDevice device, 
    VkDescriptorSetLayout layout
  );
};
