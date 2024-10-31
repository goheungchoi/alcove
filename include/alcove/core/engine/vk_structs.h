// vk_structs.h
#pragma once
#include <core/engine/vk_common.h>

namespace vkst {

  inline VkImageCreateInfo image_create_info(
    VkFormat format,
    VkExtent3D extent,
    VkImageUsageFlags usage
  ) {
    VkImageCreateInfo info{
      .sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
      .pNext = nullptr,

      .imageType = VK_IMAGE_TYPE_2D,

      .format = format,
      .extent = extent,

      .mipLevels = 1,  // The number of LOD (Levels of detail) available for minified sampling of the image
      .arrayLayers = 1, // The number of layers in the image

      // Needs for MSAA (Multisampling anti-aliasing).
      // We will not be using MSAA by default, 
      // so default it to 1 sample per pixel.
      .samples = VK_SAMPLE_COUNT_1_BIT,

      // More efficient memory access then linear tiling
      .tiling = VK_IMAGE_TILING_OPTIMAL,
      .usage = usage,

      // The initialLayout must be either VK_IMAGE_LAYOUT_UNDEFINED or 
      // VK_IMAGE_LAYOUT_PREINITIALIZED. If it is VK_IMAGE_LAYOUT_PREINITIALIZED, 
      // then the image data can be preinitialized by the host 
      // while using this layout, and the transition away from this layout 
      // will preserve that data. If it is VK_IMAGE_LAYOUT_UNDEFINED, 
      // then the contents of the data are considered to be undefined, 
      // and the transition away from this layout is not guaranteed 
      // to preserve that data. For either of these initial layouts,
      // any image subresources must be transitioned to another layout 
      // before they are accessed by the device.
      .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED
    };
    return info;
  }

  inline VkImageViewCreateInfo image_view_create_info(
    VkFormat format, 
    VkImage image, 
    VkImageAspectFlags aspectMask) {
    VkImageViewCreateInfo info {
      .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
      .pNext = nullptr,

      .image = image,
      .viewType = VK_IMAGE_VIEW_TYPE_2D,
      .format = format,
      .subresourceRange = {
        .aspectMask = aspectMask,
        .baseMipLevel = 0,
        .levelCount = 1,
        .baseArrayLayer = 0,
        .layerCount = 1,
      }
    };
    return info;
  }

  inline VkRenderingAttachmentInfo attachment_info(
    VkImageView view,
    std::optional<VkClearValue> clear,
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

  inline VkRenderingAttachmentInfo depth_attachment_info(
    VkImageView view,
    VkImageLayout layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
  ) {
    VkRenderingAttachmentInfo depthAttachment {
      .sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO,
      .pNext = nullptr,

      .imageView = view,
      .imageLayout = layout,
      
      // Clear the depth value with 0.f, as we are going to use depth 0 
      // as the "far" value, and 1 as the "near" value
      .loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
      .storeOp = VK_ATTACHMENT_STORE_OP_STORE,
      .clearValue = {
        .depthStencil = {
          .depth = 0.f
        }
      }
    };

    return depthAttachment;
  }

  inline VkRenderingInfo rendering_info(
    VkExtent2D renderExtent,
    VkRenderingAttachmentInfo* colorAttachment,
    VkRenderingAttachmentInfo* depthAttachment
  ) {
    VkRenderingInfo renderingInfo {
      .sType = VK_STRUCTURE_TYPE_RENDERING_INFO,
      .pNext = nullptr,

      .renderArea = VkRect2D{ VkOffset2D{ 0, 0 }, renderExtent },
      .layerCount = 1,
      .colorAttachmentCount = 1,
      .pColorAttachments = colorAttachment,
      .pDepthAttachment = depthAttachment,
      .pStencilAttachment = nullptr
    };

    return renderingInfo;
  }

}

