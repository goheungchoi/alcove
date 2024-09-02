// vk_structs.h
#pragma once
#include <core/engine/vk_common.h>

namespace vkst {

  VkRenderingAttachmentInfo attachment_info(
    VkImageView view,
    VkClearValue* clear,
    VkImageLayout layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
  ) {
    VkRenderingAttachmentInfo colorAttachment {
      .sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO,
      .pNext = nullptr,
      
      .imageView = view,
      .imageLayout = layout,

      // When this rendering attachment is loaded in a renderpass
      // CLEAR - Set it to the clear value and skip loading the data in the image view
      // LOAD  - Keep the data in the image view
      .loadOp = clear ? VK_ATTACHMENT_LOAD_OP_CLEAR : VK_ATTACHMENT_LOAD_OP_LOAD,
      
      // When this rendering attachment is stored in a memory
      .storeOp = VK_ATTACHMENT_STORE_OP_STORE
    };

    if (clear) {
      colorAttachment.clearValue = *clear;
    }

    return colorAttachment;
  }

  VkRenderingInfo rendering_info(
    VkExtent2D renderExtent,
    VkRenderingAttachmentInfo* colorAttachment,
    VkRenderingAttachmentInfo* depthAttachment
  ) {
    VkRenderingInfo renderingInfo {
      .sType = VK_STRUCTURE_TYPE_RENDERING_INFO,
      .pNext = nullptr,

      .renderArea = VkRect2D{ VkOffset2D{ 0, 0 }, renderExtent };
      .layerCount = 1,
      .colorAttachmentCount = 1,
      .pColorAttachments = colorAttachment,
      .pDepthAttachment = depthAttachment,
      .pStencilAttachment = nullptr
    };

    return renderingInfo;
  }

}

