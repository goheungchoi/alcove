#pragma once

#include "renderer/vulkan/context/vk_common.h"

class VulkanSwapchain {
  
  VkSurfaceCapabilitiesKHR _capabilities;
  VkSurfaceFormatKHR _surfaceFormat;
  VkPresentModeKHR _presentMode;
  VkExtent2D _extent;
  uint32_t _imageCount;
  VkSwapchainCreateInfoKHR _swapchainCreateInfoCache;

  VkSwapchainKHR _swapchain;
  uint32_t _swapchainImageIndex{ 0 };
  std::vector<VkImage> _images;
  std::vector<VkImageView> _views;
public:
  
  void Init(VkPhysicalDevice gpu, VkSurfaceKHR surface, bool allowTearing, bool vSync);

  void Build(VkDevice device, uint32_t width, uint32_t height);
  void Destroy(VkDevice device);
  void ResizeSwapchain(VkDevice device, uint32_t width, uint32_t height);

  VkFormat GetFormat() {
    return _surfaceFormat.format;
  }

  VkColorSpaceKHR GetColorSpace() {
    return _surfaceFormat.colorSpace;
  }

  VkExtent2D GetExtent() {
    return _extent;
  }

  /**
   * @brief Acquire the next available image in a swapchain.
   * 
   * @param device Vulkan device
   * @param swapchainSemaphore Semaphore to be signaled when it successfully acquires an image
   * @return VkImage An image on a swapchain. VK_NULL_HANDLE when VK_ERROR_OUT_OF_DATE_KHR occurs, 
   * meaning need to stop rendering and rebuild the swapchain to resize.
   */
  VkImage AcquireNextSwapchainImage(VkDevice device, VkSemaphore swapchainSemaphore);
  VkImage GetCurrentSwapchainImage() {
    if (_swapchainImageIndex < 0) return VK_NULL_HANDLE;
    return _images[_swapchainImageIndex];
  }
  uint32_t GetCurrentSwapchainImageIndex() {
    return _swapchainImageIndex;
  }

  operator VkSwapchainKHR() const {
    return _swapchain;
  }

private:
  struct SwapchainSupportData {
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
  };

  SwapchainSupportData QuerySwapchainSupport(VkPhysicalDevice gpu, VkSurfaceKHR surface);

  VkSurfaceFormatKHR SelectSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableSurfaceFormats);
  VkPresentModeKHR SelectPresentMode(VkPresentModeKHR preferedPresentMode, const std::vector<VkPresentModeKHR>& availablePresentModes);
  VkExtent2D SelectSwapchainExtent(uint32_t width, uint32_t height, const VkSurfaceCapabilitiesKHR& capabilities);
};
