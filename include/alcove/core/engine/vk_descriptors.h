#pragma once

#include <core/engine/vk_common.h>

/**
 * @brief A builder class to set up a descriptor set layout with bindings.
 * 
 */
struct DescriptorLayoutBuilder {
  std::vector<VkDescriptorSetLayoutBinding> bindings;

  /**
   * @brief Add a new descriptor set layout binding.
   * 
   * @param binding The binding number of this entry
   * @param type The type of resource descriptors are used for this binding
   */
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

/**
 * @brief Manages the allocation and lifecycle of descriptor sets 
 * from a descriptor pool.
 * 
 */
struct DescriptorAllocator {
  struct PoolSizeRatio {
    VkDescriptorType type;
    float ratio;
  };

  VkDescriptorPool pool;

  /**
   * @brief Initialize a descriptor pool to allocate descriptor sets.
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
