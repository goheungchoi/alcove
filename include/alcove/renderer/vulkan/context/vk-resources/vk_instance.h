#pragma once 

#include "renderer/vulkan/context/vk_common.h"

class VulkanInstance {

  VkInstance                _instance;        // Vulkan library handle
  VkDebugUtilsMessengerEXT  _debugMessenger; // Vulkan debug output handle

public:

  void Init_Win32(void* hInstance, void* hwnd);
  void Init_SDL2(struct SDL_Window* sdlWindow);

  void Cleanup();

  bool CreateSurface_SDL2(struct SDL_Window* sdlWindow, VkSurfaceKHR* outSurface);

  void FindPhysicalDevice(VkSurfaceKHR surface, const std::vector<const char*>& extensions, VkPhysicalDevice *outPhysicalDevice);

  operator VkInstance() const { return _instance; }

#ifndef NDEBUG
public:
  static VKAPI_ATTR VkBool32 VKAPI_CALL  DebugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
    void* pUserData
  );
  
private:
  inline bool CheckVulkanInstanceExtensionSupport(std::span<const char*> requiredExtensions);
  inline bool CheckVulkanValidationLayerSupport(std::span<const char*> layers);

  inline void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& debug_messenger_create_info);

  inline void SetupDebugMessenger();
  inline void CleanupDebugMessenger();

  inline VkResult CreateDebugUtilsMessengerEXT(
    VkInstance instance, 
    const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, 
    const VkAllocationCallbacks* pAllocator, 
    VkDebugUtilsMessengerEXT* pDebugMessenger) {
    auto func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
    if (func != nullptr) {
      return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
    } else {
      return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
  }
  
  inline void DestroyDebugUtilsMessengerEXT(
    VkInstance instance, 
    VkDebugUtilsMessengerEXT debugMessenger, 
    const VkAllocationCallbacks* pAllocator) {
    auto func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
    if (func != nullptr) {
      func(instance, debugMessenger, pAllocator);
    }
  }
#endif  // !NDEBUG
};


