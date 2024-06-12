#pragma once

#include <vulkan/vulkan.h>

namespace vkutil {

VkImageSubresourceRange image_subresource_range(VkImageAspectFlags aspectMask) {
  VkImageSubresourceRange subImage {};

  // Aspect either color or depth.
  subImage.aspectMask = aspectMask;

  // Subresource mipmapped images
  // From mipmap level 0 to remaining mipmap levels.
  subImage.baseMipLevel = 0;  
  subImage.levelCount = VK_REMAINING_MIP_LEVELS;

  // Subresource array images
  // From array image layer 0 to remaining array layers.
  subImage.baseArrayLayer = 0;
  subImage.layerCount = VK_REMAINING_ARRAY_LAYERS;

  return subImage;
}

/**
 * @brief Record the command to transition a image layout.
 * GPUs store images in different formats for different needs in their memory.
 * An image layout is the Vulkan abstraction over these formats.
 * To change the layout of an image, Vulkan does pipeline barriers.
 * A pipeline barrier is a way to syncronize the commands from a single command buffer,
 * but it also can do things such as 
 * 
 * @param cmdBuf 
 * @param image 
 * @param currentLayout 
 * @param newLayout 
 */
void cmd_transition_image(
  VkCommandBuffer cmdBuf, 
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

    .image = image,
    // Target the subresource of the image
    .subresourceRange = image_subresource_range(
      (newLayout == VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL) ?
      VK_IMAGE_ASPECT_DEPTH_BIT :
      VK_IMAGE_ASPECT_COLOR_BIT
    ),
  };

  VkDependencyInfo dependencyInfo {
    .sType = VK_STRUCTURE_TYPE_DEPENDENCY_INFO,
    .pNext = nullptr,

    .imageMemoryBarrierCount = 1,
    .pImageMemoryBarriers = &imageBarrier
  };

  // Syncronization-2 feature pipeline barrier
  vkCmdPipelineBarrier2(cmdBuf, &dependencyInfo);
}

}
