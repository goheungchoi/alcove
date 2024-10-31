#pragma once

#include "renderer/vulkan/context/vk_common.h"

#include "renderer/vulkan/context/vk_render_device.h"

/**
 * @brief Render context that handles 
 * 1. Preparing frames
 * 2. Submiting command buffers and draw calls
 * 3. Queue present
 * 
 */
class VkContext {
  VkRenderDevice& _device;

public:

  VkContext(VkRenderDevice& device) : _device{ device } {}




};
