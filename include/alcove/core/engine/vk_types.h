#pragma once

#include <core/engine/vk_common.h>

// TODO: Need to optimize DeletionQueue for mass deletion cases.
//			 Arrays of vk handles and delete them by looping through arrays.
struct DeletionQueue {
	std::deque<std::function<void()>> deletors;

	void push_function(std::function<void()>&& f) {
		deletors.push_back(f);
	}

	void flush() {
		for (auto f : deletors) f();
		deletors.clear();
	}
};

struct QueueData {
  VkQueue _handle;
  uint32_t _index;
};

struct FrameData {
  VkCommandPool _command_pool;
  VkCommandBuffer _main_command_buffer;
  VkSemaphore _swapchain_semaphore;  // Let render commands wait on the
                                     // swapchian image request
  VkSemaphore _render_semaphore;     // Control presenting images to the OS once
                                     // drawing finishes
  VkFence _render_fence;  // Wait for the draw commands of this frame 
													// to be finished
	DeletionQueue _local_deletion_queue;
};

struct Canvas {
	VmaAllocation _allocation;
	VkExtent3D _extent;
	VkFormat _image_format;
	VkImage _image;
	VkImageView _image_view;
};
