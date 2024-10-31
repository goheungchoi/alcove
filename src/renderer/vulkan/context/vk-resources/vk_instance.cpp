#include "renderer/vulkan/context/vk-resources/vk_instance.h"

#include <SDL.h>        // The main SDL library data for opening a window and input
#include <SDL_vulkan.h> // Vulkan-specific flags and functionality for opening a Vulkan-compatible window

#include "renderer/vulkan/context/vk-resources/vk_gpu_selector.h"

void VulkanInstance::Init_Win32(void *hInstance, void *hwnd)
{
}

void VulkanInstance::Init_SDL2(SDL_Window* window) {
  // Set the application information
  VkApplicationInfo app_info = {
    .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
    .pNext = nullptr,
    .pApplicationName = "Alcove",
    .applicationVersion = VK_MAKE_VERSION(1, 0, 0),
    .pEngineName = "No Engine",
    .engineVersion = VK_MAKE_VERSION(1, 0, 0),
    .apiVersion = VK_API_VERSION_1_3,
  };
  
  // If the SDL window is not set, throw an error.
  if (!window) throw std::runtime_error("SDL Window is not created!");

  // Extensions
  std::vector<const char*> extensions;

  // Get instance extensions from SDL library.
  unsigned int sdlExtensionCount = 0;
  SDL_Vulkan_GetInstanceExtensions(window, &sdlExtensionCount, nullptr);
  std::vector<const char*> sdlExtensions(sdlExtensionCount);
  SDL_Vulkan_GetInstanceExtensions(window, &sdlExtensionCount, sdlExtensions.data());
  extensions.insert(std::end(extensions), std::begin(sdlExtensions), std::end(sdlExtensions));

#ifndef NDEBUG 
  // DEBUG: Set up a debug messenger with a callback
  extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
  // Check if the necessary instance extensions are available
  if(!CheckVulkanInstanceExtensionSupport(extensions)) {
    throw std::runtime_error("Instance extensions required, but not available!");
  }
#endif

  // Layers
  std::vector<const char*> layers;

#ifndef NDEBUG
  // DEBUG: Get a validation layer
  layers.push_back("VK_LAYER_KHRONOS_validation");
  if (!CheckVulkanValidationLayerSupport(layers)) {
    throw std::runtime_error("Validation layers requested, but not available!");
  }
#endif

#ifndef NDEBUG
  // DEBUG: Debug util messenger creation.
  VkDebugUtilsMessengerCreateInfoEXT debugMessengerInfo;
  PopulateDebugMessengerCreateInfo(debugMessengerInfo);
#endif

  // Set the instance create info
  VkInstanceCreateInfo instanceInfo {
    .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
#ifndef NDEBUG
    .pNext = &debugMessengerInfo,
#else
    .pNext = nullptr,
#endif
    .flags = 0,
    .pApplicationInfo = &app_info,

    .enabledLayerCount = static_cast<uint32_t>(layers.size()),
    .ppEnabledLayerNames = layers.data(),

    .enabledExtensionCount = static_cast<uint32_t>(extensions.size()),
    .ppEnabledExtensionNames = extensions.data(),
  };

  // Create the instance
  VK_CHECK(vkCreateInstance(&instanceInfo, nullptr, &_instance));

#ifndef NDEBUG
  // Set up a debug messenger
  SetupDebugMessenger();
#endif
}

void VulkanInstance::Cleanup()
{
  CleanupDebugMessenger();


}

bool VulkanInstance::CreateSurface_SDL2(
  SDL_Window* sdlWindow, 
  VkSurfaceKHR* outSurface) {
  if (!SDL_Vulkan_CreateSurface(sdlWindow, _instance, outSurface)) {
    return false;
  }
  return true;
}

void VulkanInstance::FindPhysicalDevice(
  VkSurfaceKHR surface, 
  const std::vector<const char*>& extensions,
  VkPhysicalDevice *outPhysicalDevice) {
  /* TODO: Find a physical device that supports required Vulkan features.*/
  
  // Required physical device Vulkan features
  // VkPhysicalDeviceVulkan13Features features13 {
  //   .synchronization2 = true,
  //   .dynamicRendering = true,
  // };
  // VkPhysicalDeviceVulkan12Features features12 {
  //   .descriptorIndexing = true,
  //   .bufferDeviceAddress = true,
  // };

  // Select a GPU from the physical device query
  VkPhysicalDeviceSelector gpuSelector;
  *outPhysicalDevice = gpuSelector
    .query_physical_device_type(VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
    .query_device_extensions(extensions)
    ///// Required vulkan version features
    ///// Check if the swap chain support is adequate
    // is_suitable &= check_swapchain_support(gpu);
    .query_graphics_queue_family(
      VK_QUEUE_GRAPHICS_BIT, 
      VK_QUEUE_TRANSFER_BIT, 
      VK_QUEUE_COMPUTE_BIT
    )
    .query_present_queue_family(surface)
    .select_GPU(_instance)
    .get_selected_GPU();
}

#ifndef NDEBUG
VKAPI_ATTR VkBool32 VKAPI_CALL VulkanInstance::DebugCallback(
  // VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT: Diagnostic message
  // VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT: Informational message like the creation of a resource
  // VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT: Message about behavior that is not necessarily an error, but very likely a bug in your application
  // VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT: Message about behavior that is invalid and may cause crashes
  VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
  // VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT: Some event has happened that is unrelated to the specification or performance
  // VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT: Something has happened that violates the specification or indicates a possible mistake
  // VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT: Potential non-optimal use of Vulkan
  VkDebugUtilsMessageTypeFlagsEXT messageType,
  // pCallbackData->pMessage: The debug message as a null-terminated string
  // pCallbackData->pObjects: Array of Vulkan object handles related to the message
  // pCallbackData->objectCount: Number of objects in array
  const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
  // a pointer that was specified during the setup of the callback and allows you to pass your own data to it.  
  void* pUserData
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

bool VulkanInstance::CheckVulkanInstanceExtensionSupport(std::span<const char*> requiredExtensions)
{
  uint32_t extension_count{ 0 };
  vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, nullptr);
  std::vector<VkExtensionProperties> extensionProps(extension_count);
  vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, extensionProps.data());

  fmt::print("Available Extensions:\n");
  for (const auto& extension : extensionProps) {
    fmt::print("\t{}\n", extension.extensionName);
  } fmt::print("\n");

  fmt::print("Check Extension Support:\n");
  for (const char* requiredExtension : requiredExtensions) {
    fmt::print("\t{}...", requiredExtension);
    bool extensionFound = false;

    for (const auto& extension : extensionProps) {
      if (strcmp(requiredExtension, extension.extensionName) == 0) {
        fmt::print("found\n");
        extensionFound = true;
        break;
      }
    }

    if (!extensionFound) {
      fmt::print("not found!\n");
      return false;
    }
  } fmt::print("\n");

  return true;
}

inline bool VulkanInstance::CheckVulkanValidationLayerSupport(std::span<const char *> requiredLayers)
{
  uint32_t layerCount = 0;
  vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
  std::vector<VkLayerProperties> availableLayers(layerCount);
  vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

  fmt::print("Available Layers:\n");
  for (const auto& availableLayer : availableLayers) {
    fmt::print("\t{}\n", availableLayer.layerName);
  } fmt::print("\n");

  fmt::print("Check Layer Support:\n");
  for (const char* requiredLayer : requiredLayers) {
    fmt::print("\t{}...", requiredLayer);
    bool layerFound = false;

    for (const auto& availableLayer : availableLayers) {
      if (strcmp(requiredLayer, availableLayer.layerName) == 0) {
        fmt::print("found\n");
        layerFound = true;
        break;
      }
    }

    if (!layerFound) {
      fmt::print("not found!\n");
      return false;
    }
  } fmt::print("\n");

  return true;
}
inline void VulkanInstance::PopulateDebugMessengerCreateInfo(
  VkDebugUtilsMessengerCreateInfoEXT& debugMessengerCreateInfo) {
  debugMessengerCreateInfo = {
    .sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
    .messageSeverity = 
      VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
      VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
      VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT,
    .messageType = 
      VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | 
      VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | 
      VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT,
    .pfnUserCallback = DebugCallback,
    .pUserData = nullptr, // Optional
  };
}

inline void VulkanInstance::SetupDebugMessenger() {
  fmt::print("Debug Mode is On!\n");
  fmt::print("Set up the debug messenger...");

  VkDebugUtilsMessengerCreateInfoEXT debugMessengerInfo;
  PopulateDebugMessengerCreateInfo(debugMessengerInfo);

  VK_CHECK(
    CreateDebugUtilsMessengerEXT(
      _instance, 
      &debugMessengerInfo, 
      nullptr, 
      &_debugMessenger
    )
  ); 
  fmt::print("done\n");
}

inline void VulkanInstance::CleanupDebugMessenger() {
  fmt::print("Clean up a debug messenger...");

  DestroyDebugUtilsMessengerEXT(_instance, _debugMessenger, nullptr);
  fmt::print("done\n");
}
#endif
 