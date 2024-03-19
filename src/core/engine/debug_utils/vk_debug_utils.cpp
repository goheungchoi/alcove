#include "core/engine/debug_utils/vk_debug_utils.h"

#include <cstring>

bool DebugUtils::_is_init{ false };
VkDebugUtilsMessengerEXT DebugUtils::debugMessenger;

bool DebugUtils::checkInstanceExtensionSupport(std::vector<const char*> exts) {
  uint32_t extension_count = 0;
  vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, nullptr);
  std::vector<VkExtensionProperties> extensions(extension_count);
  vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, extensions.data());

  fmt::print("Available Extensions:\n");
  for (const auto& extension : extensions) {
    fmt::print("\t{}\n", extension.extensionName);
  }

  fmt::print("\n");

  fmt::print("Check Extension Support:\n");
  for (const char* ext_name : exts) {
    fmt::print("\t{}...", ext_name);
    bool extension_found = false;

    for (const auto& extension : extensions) {
      if (strcmp(ext_name, extension.extensionName) == 0) {
        fmt::print("found\n");
        extension_found = true;
        break;
      }
    }

    if (!extension_found) {
      fmt::print("not found!\n");
      return false;
    }
  }

  fmt::print("\n");

  return true;
}

bool DebugUtils::checkValidationLayerSupport(std::vector<const char*> layers) {
  uint32_t layer_count = 0;
  vkEnumerateInstanceLayerProperties(&layer_count, nullptr);
  std::vector<VkLayerProperties> available_layers(layer_count);
  vkEnumerateInstanceLayerProperties(&layer_count, available_layers.data());

  fmt::print("Available Layers:\n");
  for (const auto& layer_props : available_layers) {
    fmt::print("\t{}\n", layer_props.layerName);
  }

  fmt::print("\n");

  fmt::print("Check Layer Support:\n");
  for (const char* layer_name : layers) {
    fmt::print("\t{}...", layer_name);
    bool layer_found = false;

    for (const auto& layer_props : available_layers) {
      if (strcmp(layer_name, layer_props.layerName) == 0) {
        fmt::print("found\n");
        layer_found = true;
        break;
      }
    }

    if (!layer_found) {
      fmt::print("not found!\n");
      return false;
    }
  }
  
  fmt::print("\n");

  return true;
}

VKAPI_ATTR VkBool32 VKAPI_CALL DebugUtils::debugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
  // VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT: Diagnostic message
  // VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT: Informational message like the creation of a resource
  // VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT: Message about behavior that is not necessarily an error, but very likely a bug in your application
  // VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT: Message about behavior that is invalid and may cause crashes
    VkDebugUtilsMessageTypeFlagsEXT messageType,
  // VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT: Some event has happened that is unrelated to the specification or performance
  // VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT: Something has happened that violates the specification or indicates a possible mistake
  // VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT: Potential non-optimal use of Vulkan
    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
  // pCallbackData->pMessage: The debug message as a null-terminated string
  // pCallbackData->pObjects: Array of Vulkan object handles related to the message
  // pCallbackData->objectCount: Number of objects in array
    void* pUserData
  // a pointer that was specified during the setup of the callback and allows you to pass your own data to it.  
  ) {

    fmt::println("Validation Layer: {}", pCallbackData->pMessage);

    // The callback returns a boolean that indicates 
    // if the Vulkan call that triggered the validation layer message 
    // should be aborted. 
    // If the callback returns true, 
    // then the call is aborted with the VK_ERROR_VALIDATION_FAILED_EXT error. 
    // This is normally only used to test the validation layers themselves, 
    // so it should always return VK_FALSE.
    return VK_FALSE;
  }
