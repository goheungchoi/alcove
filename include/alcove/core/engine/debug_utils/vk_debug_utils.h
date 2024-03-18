// vk_debug_msg.h
#pragma once

#include <vk_common.h>

class DebugUtils {
  DebugUtils() = delete;
  DebugUtils(DebugUtils&) = delete;
  DebugUtils(DebugUtils&&) = delete;
  DebugUtils& operator=(const DebugUtils&) = delete;
  DebugUtils& operator=(DebugUtils&&) = delete;

public:
  static bool checkInstanceExtensionSupport(std::vector<const char*> exts);
  static bool checkValidationLayerSupport(std::vector<const char*> layers);

private:
  static bool _is_init;
  static VkDebugUtilsMessengerEXT debugMessenger;
public:
  static VKAPI_ATTR VkBool32 VKAPI_CALL  debugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
    void* pUserData
  );

  static inline void populateDebugMessengerCreateInfo(
    VkDebugUtilsMessengerCreateInfoEXT& debug_messenger_create_info
  ) {
    debug_messenger_create_info = {
      .sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
      .messageSeverity = 
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT,
      .messageType = 
        VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | 
        VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | 
        VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT,
      .pfnUserCallback = debugCallback,
      .pUserData = nullptr, // Optional
    };
  }

  static inline void setupDebugMessenger(VkInstance& _instance) {
    // Make sure this setup function is called only once in the program.
    assert(_is_init == false);
    
    VkDebugUtilsMessengerCreateInfoEXT debug_messenger_create_info;
    populateDebugMessengerCreateInfo(debug_messenger_create_info);

    VK_CHECK(CreateDebugUtilsMessengerEXT(
      _instance, 
      &debug_messenger_create_info, 
      nullptr, 
      &debugMessenger
    ));

    _is_init = true;
  }

  static inline void cleanupDebugMessenger(VkInstance& _instance) {
    assert(_is_init == true);

    DestroyDebugUtilsMessengerEXT(_instance, debugMessenger, nullptr);

    _is_init = false;
  }

private:
  static inline VkResult CreateDebugUtilsMessengerEXT(
    VkInstance instance, 
    const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, 
    const VkAllocationCallbacks* pAllocator, 
    VkDebugUtilsMessengerEXT* pDebugMessenger
  ) {
    auto func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
    if (func != nullptr) {
      return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
    } else {
      return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
  }
  
  static inline void DestroyDebugUtilsMessengerEXT(
    VkInstance instance, 
    VkDebugUtilsMessengerEXT debugMessenger, 
    const VkAllocationCallbacks* pAllocator
  ) {
    auto func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
    if (func != nullptr) {
      func(instance, debugMessenger, pAllocator);
    }
  }
};
