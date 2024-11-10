#pragma once

#include "renderer/vulkan/context/vk_common.h"

#include "renderer/vulkan/context/vk-resources/vk_instance.h"

#include "renderer/vulkan/context/vk-resources/vk_swapchain.h"

#include "renderer/vulkan/context/vk_types.h"

/**
 * @brief 
 * 
 */
class VkRenderDevice {

  struct SDL_Window* _window{ nullptr };  // Forward-declared SDL_Window to be attached

  VulkanInstance            _instance;
  VkPhysicalDevice          _gpu;     // GPU chosen as the default device
  VkDevice                  _device;          // Vulkan device for commands
  Vk::Queue							_graphicsQueue;		// Device graphics queue struct
  Vk::Queue							_presentQueue;		// Device presentation queue struct
  VkSurfaceKHR              _surface;         // Vulkan window surface

  VmaAllocator _allocator;

  VulkanSwapchain _swapchain;
public:

  void Init_Win32(void* hInstance, void* hwnd);
  void Init_SDL2(void* sdlWindow);

  void Cleanup();

private:

  const std::vector<const char*> _deviceExtensions {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
  };

  void CreateDeviceAndQueues();
  void DestroyDeviceAndQueues();

  int GetGraphicsQueueFamilyIndex(VkPhysicalDevice gpu);
  int GetPresentQueueFamilyIndex(VkPhysicalDevice gpu, VkSurfaceKHR surface);
  
  void CreateVmaAllocator();
  void DestroyVmaAllocator();

private:

  Vk::Image CreateImage(
    VkExtent3D size,
    VkFormat format,
    VkImageUsageFlags usage,
    bool mipmap
  );

};
