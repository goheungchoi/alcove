#pragma once

#include "renderer/vulkan/context/vk_common.h"

namespace VkStruct {

  inline VkImageCreateInfo ImageCreateInfo(
    VkFormat format,
    VkExtent3D extent,
    uint32_t mipLevels,
    uint32_t arrayLayers,
    VkSampleCountFlagBits sampleCountFlags,
    VkImageUsageFlags usage) {
    VkImageCreateInfo info{
      .sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
      .pNext = nullptr,

      .imageType = VK_IMAGE_TYPE_2D,

      .format = format,
      .extent = extent,

      .mipLevels = mipLevels,  // The number of LOD (Levels of detail) available for minified sampling of the image
      .arrayLayers = arrayLayers, // The number of layers in the image

      // Needs for MSAA (Multisampling anti-aliasing).
      // We will not be using MSAA by default, 
      // so default it to 1 sample per pixel.
      .samples = sampleCountFlags,

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

  inline VkImageCreateInfo ImageCreateInfo(
    VkFormat format,
    VkExtent3D extent,
    VkImageUsageFlags usage) {
    return ImageCreateInfo(format, extent, 1, 1, VK_SAMPLE_COUNT_1_BIT, usage);
  }

  inline VkImageViewCreateInfo ImageViewCreateInfo(
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

}