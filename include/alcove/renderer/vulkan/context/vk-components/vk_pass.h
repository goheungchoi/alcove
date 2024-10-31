#pragma once

#include "renderer/vulkan/context/vk_common.h"

// Pass types?
// Opacity, Transparency, ...

/**
 * @brief Render pass
 * 
 */
class VulkanRenderPass {

public:

  void BeginRendering();

  void WriteDescriptorSet();

  void BindPipeline();

  void PushConstants();

  void BindIndexBuffer();


};
