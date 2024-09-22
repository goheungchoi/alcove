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
   * @brief Initialize a descriptor pool to allocate descriptor sets.
   * 
   * @param device The GPU device
   * @param maxSets The number of max sets
   * @param poolRatios The ratio of the types of descriptor pools
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

/**
 * @brief Manages the allocation and lifecycle of descriptor sets 
 * from a descriptor pool. This one is useful when we don't know what
 * descriptor sets will be needed when loading arbitrary object files.
 * 
 */
class GrowableDescriptorAllocator {
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

  // The array of pool size ratios for reallocation of pools
  std::vector<PoolSizeRatio> ratios;
  // The descriptor pools that have no empty space
  std::vector<VkDescriptorPool> fullPools;
  // The descriptor pools that can still be used
  std::vector<VkDescriptorPool> readyPools;
  // How many descriptor sets we allocate per pool
  uint32_t setsPerPool;

public:
  /**
   * @brief Initialize a descriptor pool to allocate descriptor sets.
   * 
   * @param device The GPU device
   * @param initialSets The number of initial sets
   * @param poolRatios The ratio of the types of descriptor pools
   */
  void init_pool(
    VkDevice device, 
    uint32_t initialSets,
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

private:
  // Pick up a pool from the ready pools, or create a new one if none is available.
  VkDescriptorPool get_pool(VkDevice device);
  VkDescriptorPool create_pool(VkDevice device, uint32_t setCount, std::span<PoolSizeRatio> poolRatios);
};

