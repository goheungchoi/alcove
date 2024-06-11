#pragma once

#include <vulkan/vulkan.h>

namespace vkutil {

void image_subresource_range(VkImageAspectFlags aspectMask) {
  VkImageSubresourceRange subImage {};
  subImage.aspectMask = aspectMask;
  subImage.baseMipLevel = 0;
  subImage.levelCount = VK_REMAINING_MIP_LEVELS;
  subImage.baseArrayLayer = 0;
  subImage.layerCount = VK_REMAINING_ARRAY_LAYERS;
}

void transition_image(
  VkCommandBuffer cmd, 
  VkImage image, 
  VkImageLayout currentLayout,
  VkImageLayout newLayout
) {
  VkImageMemoryBarrier2 imageBarrier {
    .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER_2,
    .pNext = nullptr,

    .srcStageMask = VK_PIPELINE_STAGE_2_ALL_COMMANDS_BIT,
    .srcAccessMask = VK_ACCESS_2_MEMORY_WRITE_BIT,

    .dstStageMask = VK_PIPELINE_STAGE_2_ALL_COMMANDS_BIT,
    .dstAccessMask = VK_ACCESS_2_MEMORY_WRITE_BIT | VK_ACCESS_2_MEMORY_READ_BIT,

    .oldLayout = currentLayout,
    .newLayout = newLayout,


  };



}

}
