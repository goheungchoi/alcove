#include "renderer/vulkan/context/vk-resources/vk_swapchain.h"

#include "renderer/vulkan/context/vk_structs.h"


void VulkanSwapchain::Init(VkPhysicalDevice gpu, VkSurfaceKHR surface, bool allowTearing, bool VSync)
{
  SwapchainSupportData data = QuerySwapchainSupport(gpu, surface);

  _capabilities = data.capabilities;
  _surfaceFormat = SelectSurfaceFormat(data.formats);

  VkPresentModeKHR preferedPresentMode;
  if (!allowTearing && VSync) {
    preferedPresentMode = VK_PRESENT_MODE_MAILBOX_KHR;
  } else if (allowTearing && VSync) {
    preferedPresentMode = VK_PRESENT_MODE_FIFO_RELAXED_KHR;
  } else {
    preferedPresentMode = VK_PRESENT_MODE_IMMEDIATE_KHR;
  }
  _presentMode = SelectPresentMode(preferedPresentMode, data.presentModes);
  
  _extent = _capabilities.currentExtent;

  uint32_t imageCount = data.capabilities.minImageCount + 1;
  if (0 < data.capabilities.maxImageCount && data.capabilities.maxImageCount < imageCount) {
    imageCount = data.capabilities.maxImageCount;
  }
  _imageCount = imageCount;

  // Fill out the create info cache
  _swapchainCreateInfoCache = {
    .sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
    .surface = surface,
    .minImageCount = _imageCount,
    .imageFormat = _surfaceFormat.format,
    .imageColorSpace = _surfaceFormat.colorSpace,
    .imageExtent = _extent,
    .imageArrayLayers = 1,
    .imageUsage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,

    // Need to be changed if the graphics queue family index
    // and the present queue family index are not the same
    .imageSharingMode = VK_SHARING_MODE_EXCLUSIVE,
    .queueFamilyIndexCount = 0,
    .pQueueFamilyIndices = nullptr,

    .preTransform = _capabilities.currentTransform,

    .compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,

    .presentMode = _presentMode,
    .clipped = VK_TRUE,

    .oldSwapchain = VK_NULL_HANDLE
  };
}

void VulkanSwapchain::Build(VkDevice device, uint32_t width, uint32_t height)
{
  // Resize the swapchain images
  _extent = SelectSwapchainExtent(width, height, _capabilities);
  _swapchainCreateInfoCache.imageExtent = _extent;

  VK_CHECK(
    vkCreateSwapchainKHR(
      device, 
      &_swapchainCreateInfoCache, 
      nullptr, 
      &_swapchain
    )
  );

  ///// Retrieve the swap chain images
  vkGetSwapchainImagesKHR(device, _swapchain, &_imageCount, nullptr);
  _images.resize(_imageCount);
  vkGetSwapchainImagesKHR(device, _swapchain, &_imageCount, _images.data());
  
  ///// Retrieve the swap chain image views
  _views.resize(_imageCount);
  for (size_t i=0; i<_imageCount; ++i) {
    VkImageViewCreateInfo create_info = {
      .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
      .image = _images[i],
      .viewType = VK_IMAGE_VIEW_TYPE_2D,
      .format = _surfaceFormat.format,
      .components = {
        .r = VK_COMPONENT_SWIZZLE_IDENTITY, 
        .g = VK_COMPONENT_SWIZZLE_IDENTITY, 
        .b = VK_COMPONENT_SWIZZLE_IDENTITY, 
        .a = VK_COMPONENT_SWIZZLE_IDENTITY, 
      },
      .subresourceRange = {
        .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
        .baseMipLevel = 0,
        .levelCount = 1,
        .baseArrayLayer = 0,
        .layerCount = 1,
      }
    };

    VK_CHECK(
      vkCreateImageView(
        device, 
        &create_info, 
        nullptr, 
        &_views[i]
      )
    );
  }
}

void VulkanSwapchain::Destroy(VkDevice device)
{
  vkDestroySwapchainKHR(device, _swapchain, nullptr);

  // Destroy image views
  for (auto& imageView : _views)
    vkDestroyImageView(device, imageView, nullptr);
}

void VulkanSwapchain::ResizeSwapchain(VkDevice device, uint32_t width, uint32_t height)
{
  vkDeviceWaitIdle(device);

  Destroy(device);

  Build(device, width, height);
}

VkImage VulkanSwapchain::AcquireNextSwapchainImage(VkDevice device, VkSemaphore swapchainSemaphore)
{
  VkResult e = vkAcquireNextImageKHR(
    device, 
    _swapchain, 
    1'000'000'000,  // 1 sec = 1'000'000'000 ns
    swapchainSemaphore,  // Signal when successfully acquires an image
    nullptr, 
    &_swapchainImageIndex
  );
  if (e == VK_ERROR_OUT_OF_DATE_KHR) {
    return VK_NULL_HANDLE;
  }

  return _images[_swapchainImageIndex];
}

VulkanSwapchain::SwapchainSupportData VulkanSwapchain::QuerySwapchainSupport(VkPhysicalDevice gpu, VkSurfaceKHR surface)
{
  SwapchainSupportData data;

  // Fetch the basic surface capabilities
  vkGetPhysicalDeviceSurfaceCapabilitiesKHR(gpu, surface, &data.capabilities);

  // Query the supported surface formats
  uint32_t formatCount;
  vkGetPhysicalDeviceSurfaceFormatsKHR(gpu, surface, &formatCount, nullptr);
  if (formatCount == 0) {
    throw std::exception("No supported physical device surface formats found!");
  }
  data.formats.resize(formatCount);
  vkGetPhysicalDeviceSurfaceFormatsKHR(gpu, surface, &formatCount, data.formats.data());


  // Query the supported presentation modes
  uint32_t presentModeCount;
  vkGetPhysicalDeviceSurfacePresentModesKHR(gpu, surface, &presentModeCount, nullptr);
  if (presentModeCount != 0) {
    throw std::exception("No supported physical device present modes found!");
  }
  data.presentModes.resize(presentModeCount);
  vkGetPhysicalDeviceSurfacePresentModesKHR(gpu, surface, &presentModeCount, data.presentModes.data());

  return data;
}

VkSurfaceFormatKHR VulkanSwapchain::SelectSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableSurfaceFormats)
{
  for (const auto& availableFormat : availableSurfaceFormats) {
    if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB &&
      availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
      return availableFormat;
  }
  return availableSurfaceFormats[0];
}

VkPresentModeKHR VulkanSwapchain::SelectPresentMode(VkPresentModeKHR preferedPresentMode, const std::vector<VkPresentModeKHR>& availablePresentModes)
{
  for (const auto& availablePresentMode : availablePresentModes) {
    // Check if VK_PRESENT_MODE_MAILBOX_KHR is available,
    // and if yes, use the mail box mode.
    if (availablePresentMode == preferedPresentMode)
      return availablePresentMode;
  }
  return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D VulkanSwapchain::SelectSwapchainExtent(uint32_t width, uint32_t height, const VkSurfaceCapabilitiesKHR& capabilities)
{
  if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
    return capabilities.currentExtent;
  
  // When the currentExtent.width is the max value of uint32_t,
  // the current extent can differ.

  VkExtent2D extent{ width, height };

  extent.width = std::clamp(extent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
  extent.height = std::clamp(extent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

  return extent;
}
