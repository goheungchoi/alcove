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
  // How many descriptor sets the next pool will allocate
  uint32_t setsPerPool;

  // Grow rate of pools
  static constexpr float GROW_RATE{ 1.5f };
  // Max number of sets per pool
  static constexpr unsigned int MAX_DESCRIPTOR_SETS_PER_POOL{ 4092u };

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
    VkDescriptorSetLayout layout,
    void* pNext
  );

private:
  // Pick up a pool from the ready pools, or create a new one if none is available.
  VkDescriptorPool get_pool(VkDevice device);
  VkDescriptorPool create_pool(VkDevice device, uint32_t setCount, std::span<PoolSizeRatio> poolRatios);
};


// TODO: Description
class DescriptorWriter {
  std::deque<VkDescriptorImageInfo> imageInfos;
  std::deque<VkDescriptorBufferInfo> bufferInfos;
  std::vector<VkWriteDescriptorSet> writes;

public:
  /**
   * @brief 
   * 
   * @param binding 
   * @param image 
   * @param sampler 
   * @param layout The best layout to use for accessing textures in the shaders is VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL.
   * Or, when we are using textures from compute shaders and writing onto them, the layout should be VK_IMAGE_LAYOUT_GENERAL.
   * @param type VK_DESCRIPTOR_TYPE_SAMPLER is JUST the sampler, so it does not need ImageView or layout to be set.
   * VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE doesnt need the sampler set because its going to be accessed with different samplers within the shader, this descriptor type is just a pointer to the image.
   * VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER needs everything set, as it holds the information for both the sampler, and the image it samples. This is a useful type because it means we only need 1 descriptor binding to access the texture.
   * VK_DESCRIPTOR_TYPE_STORAGE_IMAGE was used back in chapter 2, it does not need sampler, and its used to allow compute shaders to directly access pixel data.
   * In both the write_image and write_buffer functions, we are being overly generic. This is done for simplicity, but if you want, you can add new ones like write_sampler() where it has VK_DESCRIPTOR_TYPE_SAMPLER and sets imageview and layout to null, and other similar abstractions.
   */
  void write_image(int binding, VkImageView image, VkSampler sampler, VkImageLayout layout, VkDescriptorType type);
  /**
   * @brief 
   * 
   * @param binding 
   * @param buffer 
   * @param size 
   * @param offset 
   * @param type The descriptor type must be one of the followings; 
   * VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER
   * VK_DESCRIPTOR_TYPE_STORAGE_BUFFER
   * VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC
   * VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC
   */
  void write_buffer(int binding, VkBuffer buffer, std::size_t size, std::size_t offset, VkDescriptorType type);

  void clear();
  void update_set(VkDevice device, VkDescriptorSet set);
};

