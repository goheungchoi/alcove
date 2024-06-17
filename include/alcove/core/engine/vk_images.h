#pragma once

#include <vulkan/vulkan.h>

// VkFenceCreateInfo vkinit::fence_create_info(VkFenceCreateFlags flags /*= 0*/)
// {
//   VkFenceCreateInfo info = {};
//   info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
//   info.pNext = nullptr;
//   info.flags = flags;
//   return info;
// }

// VkSemaphoreCreateInfo vkinit::semaphore_create_info(VkSemaphoreCreateFlags flags /*= 0*/)
// {
//   VkSemaphoreCreateInfo info = {};
//   info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
//   info.pNext = nullptr;
//   info.flags = flags;
//   return info;
// }

// VkCommandBufferBeginInfo vkinit::command_buffer_begin_info(VkCommandBufferUsageFlags flags /*= 0*/)
// {
//   VkCommandBufferBeginInfo info = {};
//   info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
//   info.pNext = nullptr;
//   info.pInheritanceInfo = nullptr;
//   info.flags = flags;
//   return info;
// }

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


void copy_image(
  VkCommandBuffer cmdBuf,
  VkImage src,
  VkExtent2D srcExtent,
  VkImage dst,
  VkExtent2D dstExtent
) {
  // TODO: Need to replace this copy logic with a fragment shader.

  VkImageBlit2 blitRegion{
    .sType = VK_STRUCTURE_TYPE_IMAGE_BLIT_2,
    .pNext = nullptr
  };

  blitRegion.srcOffsets[1].x = srcExtent.width;
	blitRegion.srcOffsets[1].y = srcExtent.height;
	blitRegion.srcOffsets[1].z = 1;

	blitRegion.dstOffsets[1].x = dstExtent.width;
	blitRegion.dstOffsets[1].y = dstExtent.height;
	blitRegion.dstOffsets[1].z = 1;

	blitRegion.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	blitRegion.srcSubresource.baseArrayLayer = 0;
	blitRegion.srcSubresource.layerCount = 1;
	blitRegion.srcSubresource.mipLevel = 0;

	blitRegion.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	blitRegion.dstSubresource.baseArrayLayer = 0;
	blitRegion.dstSubresource.layerCount = 1;
	blitRegion.dstSubresource.mipLevel = 0;

  VkBlitImageInfo2 blitInfo{ .sType = VK_STRUCTURE_TYPE_BLIT_IMAGE_INFO_2, .pNext = nullptr };
	blitInfo.dstImage = dst;
	blitInfo.dstImageLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
	blitInfo.srcImage = src;
	blitInfo.srcImageLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
	blitInfo.filter = VK_FILTER_LINEAR;
	blitInfo.regionCount = 1;
	blitInfo.pRegions = &blitRegion;

  vkCmdBlitImage2(cmdBuf, &blitInfo);
}

}
