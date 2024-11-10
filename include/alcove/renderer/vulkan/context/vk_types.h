#pragma once

#include "renderer/vulkan/context/vk_common.h"

namespace Vk {

struct Queue {
  VkQueue handle;
  uint32_t index;
};

struct Image {
	VkImage image;
	VkImageView view;
	VkExtent3D extent;
	VkFormat format;
  VmaAllocation allocation;
};

struct Buffer {
  VkBuffer buffer;
	VmaAllocation allocation;
	VmaAllocationInfo info;
};

struct FrameData {
	VkCommandPool commandPool;
  VkCommandBuffer mainCommandBuffer;
  VkSemaphore swapchainSemaphore;  // Let render commands wait on the
                                   // swapchian image request
  VkSemaphore renderSemaphore;     // Control presenting images to the OS once
                                   // drawing finishes
  VkFence renderFence;  // Wait for the draw commands of this frame 
												// to be finished
};





};
