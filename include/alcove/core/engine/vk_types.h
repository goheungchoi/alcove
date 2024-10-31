#pragma once

#include <core/engine/vk_common.h>
#include <core/engine/vk_descriptors.h>

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
	GrowableDescriptorAllocator _frame_descriptor_allocator;
};

struct GPUImage {
	VmaAllocation _allocation;
	VkExtent3D _extent;
	VkFormat _image_format;
	VkImage _image;
	VkImageView _image_view;
};

// Uniform constant layout in the compute shader
struct ComputePushConstants {
  glm::vec4 data1;
  glm::vec4 data2;
  glm::vec4 data3;
  glm::vec4 data4;
};

// Compute pipeline struct
struct ComputeEffect {
	const char* name;

	VkPipeline pipeline;
	VkPipelineLayout pipelineLayout;

	ComputePushConstants data;
};

// VkBuffer allocated from a Vma allocator
struct GPUBuffer {
	VkBuffer buffer;
	VmaAllocation allocation;
	VmaAllocationInfo info;
};

struct Vertex {
	glm::vec3 position;
	float uv_x;
	glm::vec3 normal;
	float uv_y;
	glm::vec4 color;
};

// Holds the resources needed for a mesh
struct GPUMeshBuffers {
	GPUBuffer vertexBuffer;
	GPUBuffer indexBuffer;
	VkDeviceAddress vertexBufferAddress;
};

// Push constants for our mesh object draws
struct GPUDrawPushConstants {
	glm::mat4 worldMatrix;
	VkDeviceAddress vertexBuffer;
};


struct GPUSceneData {
	glm::mat4 view;
	glm::mat4 proj;
	glm::mat4 viewProj;
	glm::vec4 ambientColor;
	glm::vec4 sunlightDir;	// w for sun power
	glm::vec4 sunlightColor;
};

// Material
enum class MaterialPass : uint8_t {
	Opaque,
	Transparent,
	Other
};

struct MaterialPipeline {
	VkPipeline pipeline;
	VkPipelineLayout layout;
};

struct MaterialInstance {
	MaterialPipeline* pipeline;
	VkDescriptorSet materialSet;
	MaterialPass passType;
};
