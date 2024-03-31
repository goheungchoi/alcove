// vk_engine.cpp
#include "core/engine/vk_engine.h"

#include <SDL.h>        // The main SDL library data for opening a window and input
#include <SDL_vulkan.h> // Vulkan-specific flags and functionality for opening a Vulkan-compatible window

#include <core/engine/vk_initializers.h>
#include <core/engine/vk_common.h>

#include <chrono>
#include <thread>

#ifdef NDEBUG /* RELEASE MODE */
using debug_t = std::integral_constant<bool, false>;
#else /*** DEBUG MODE IS ON ***/
using debug_t = std::integral_constant<bool, true>;
#endif // NDEBUG

constexpr bool bUseValidationLayers = false;

VulkanEngine* loadedEngine = nullptr; // The pointer to the vulkan engine singleton.

VulkanEngine& VulkanEngine::Get() { return *loadedEngine; } // The global reference of the vulkan engin singleton.
                                                            // Need to control explicitly when the class is initialized and destroyed.

void VulkanEngine::init() {
  // Only one engine initialization is allowed with the application.
  assert(loadedEngine == nullptr);
  loadedEngine = this;

  ///// Initialize SDL and create a window with it.
  SDL_Init(SDL_INIT_VIDEO);

  SDL_WindowFlags window_flags = (SDL_WindowFlags) (SDL_WINDOW_VULKAN);

  _window = SDL_CreateWindow(   // Create a window and store on the `_window` member var
    "Alcove",
    SDL_WINDOWPOS_UNDEFINED,
    SDL_WINDOWPOS_UNDEFINED,
    _windowExtent.width,
    _windowExtent.height,
    window_flags
  );

  if (!_window) {
    fmt::println("{}", SDL_GetError());
  }

  ///// Initialize Vulkan
  try {
    init_vulkan();

    init_swapchain();

    init_commands();

    init_sync_structures();
  } catch (const std::exception& e) {
    fmt::println("{}", e.what());
  }
  


  // Notify that every initialization step went fine.
  _isInitialized = true;
}


void VulkanEngine::cleanup() {
  if (_isInitialized) {

    destroy_swapchain();

    vkDestroySurfaceKHR(_instance, _surface, nullptr);
    vkDestroyDevice(_device, nullptr);

/*** DEBUG: Destroy the debug messenger ***/
if constexpr (debug_t::value) cleanupDebugMessenger();
/************************************************************************/
    vkDestroyInstance(_instance, nullptr);
    SDL_DestroyWindow(_window); // Destory the window
  }

  // Clear the engine singleton pointer
  loadedEngine = nullptr;
}

void VulkanEngine::draw() {

}

void VulkanEngine::run() {
  SDL_Event e;
  bool bQuit = false;

  // Main loop
  while (!bQuit) {
    // Handle events on the queue
    while (SDL_PollEvent(&e)) {
      // Close the window when user alt-f4s or clicks the X button
      if (e.type == SDL_QUIT)
        bQuit = true;

      if (e.type == SDL_WINDOWEVENT) {
        if (e.window.event == SDL_WINDOWEVENT_MINIMIZED) {
          stop_rendering = true;
        }
        if (e.window.event == SDL_WINDOWEVENT_RESTORED) {
          stop_rendering = false;
        }
      }
    }

    // Update the state

    // Draw the current frame
    //// do not draw if we are minimized
    if (stop_rendering) {
      // Throttle the speed to avoid the endless spinning
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
      continue;
    }
    draw();

  }
}

void VulkanEngine::init_vulkan()
{
  ///// Set the application information
  VkApplicationInfo app_info = {
    .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
    .pNext = nullptr,
    .pApplicationName = "Alcove",
    .applicationVersion = VK_MAKE_VERSION(1, 0, 0),
    .pEngineName = "No Engine",
    .engineVersion = VK_MAKE_VERSION(1, 0, 0),
    .apiVersion = VK_API_VERSION_1_3,
  };

  ///// Get instance extensions from SDL library.
  // If the SDL window is not set, throw an error.
  if (!_window) throw std::runtime_error("SDL Window is not created!");
  unsigned int ext_count = 0;
  SDL_Vulkan_GetInstanceExtensions(_window, &ext_count, nullptr);
  std::vector<const char*> exts(ext_count);
  SDL_Vulkan_GetInstanceExtensions(_window, &ext_count, exts.data());



/*** DEBUG: Set up a debug messenger with a callback ********************/
if constexpr (debug_t::value) exts.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
/************************************************************************/
/*** DEBUG: Check if the necessary instance extensions are available. ***/
if (debug_t::value && !checkInstanceExtensionSupport(exts)) {
  throw std::runtime_error("Instance extensions required, but not available!");
}
/************************************************************************/



  ///// Get a validation layer.
  std::vector<const char*> layers;
  [&layers]{ // Add the validation layer when debug mode.
    if constexpr (debug_t::value) /*** DEBUG: ***/
      layers.push_back("VK_LAYER_KHRONOS_validation");
  }();



/*** DEBUG: Check if the requested validation layer is available. ***/ 
if (debug_t::value && !checkValidationLayerSupport(layers)) {
  throw std::runtime_error("Validation layers requested, but not available!");
}
/********************************************************************/



  ///// Set the instance create info
  VkDebugUtilsMessengerCreateInfoEXT debug_messenger_create_info;
  VkInstanceCreateInfo instance_info = {
    .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
    .pNext = [this, &debug_messenger_create_info] { // Compile-time assignment
        if constexpr (debug_t::value) { /*** DEBUG: ***/
          populateDebugMessengerCreateInfo(debug_messenger_create_info);
          return (VkDebugUtilsMessengerCreateInfoEXT*) &debug_messenger_create_info;
        } else {
          return nullptr;
        }
      }(),
    .flags = 0,
    .pApplicationInfo = &app_info,
    .enabledLayerCount = [&layers] {    // Compile-time assignment
        if constexpr (debug_t::value) /*** DEBUG: ***/
          return static_cast<uint32_t>(layers.size());
        else 
          return 0;
      }(),
    .ppEnabledLayerNames = [&layers] {  // Compile-time assignment
        if constexpr (debug_t::value) /*** DEBUG: ***/
          return layers.data();
        else 
          return;
      }(),
    .enabledExtensionCount = static_cast<uint32_t>(exts.size()),
    .ppEnabledExtensionNames = exts.data(),
  };

  ///// Create the instance
  VK_CHECK(vkCreateInstance(&instance_info, nullptr, &_instance));



/*** DEBUG: Set up a debug messenger when debug mode. **************/ 
if constexpr (debug_t::value) setupDebugMessenger();
/********************************************************************/



  ///// Create a VkSurfaceKHR object from the SDL window
  // The actual window we will be rendering to
  if (!SDL_Vulkan_CreateSurface(_window, _instance, &_surface)) {
    throw std::runtime_error("Failed to create a SDL window surface!");
  }

  ///// Select a GPU with certain features
  uint32_t gpus_count = 0;
  VK_CHECK(vkEnumeratePhysicalDevices(_instance, &gpus_count, nullptr));
  if (!gpus_count) {
    throw std::runtime_error("GPUs supporting Vulkan not found!");
  }
  std::vector<VkPhysicalDevice> gpus(gpus_count);
  VK_CHECK(vkEnumeratePhysicalDevices(_instance, &gpus_count, gpus.data()));

  /* TODO: Find a physical device that supports required Vulkan features.*/
  const std::vector<const char*> device_extensions = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
  };

  // Select a GPU from the physical device query
  struct _device_info {
    VkPhysicalDevice _gpu;
    std::optional<uint32_t> _graphics_family_index;
    std::optional<uint32_t> _present_family_index;
  };
  std::vector<_device_info> suitable_gpus;
  auto physical_device_selector = [this, &gpus, &device_extensions](std::vector<_device_info>& suitable_gpus) {
    
    ///// Device extension checker definition
    auto check_device_extension_support = [&device_extensions](VkPhysicalDevice gpu) {
      uint32_t extension_count;
      vkEnumerateDeviceExtensionProperties(gpu, nullptr, &extension_count, nullptr);

      std::vector<VkExtensionProperties> available_extensions(extension_count);
      vkEnumerateDeviceExtensionProperties(gpu, nullptr, &extension_count, available_extensions.data());

      std::set<std::string> required_extensions(device_extensions.begin(), device_extensions.end());
      for (const auto& extension : available_extensions) {
        required_extensions.erase(extension.extensionName);
      }

      return required_extensions.empty();
    };

    ///// Check swapchain support
    

    ///// Required vulkan version features
    VkPhysicalDeviceVulkan13Features features13 {
      .synchronization2 = true,
      .dynamicRendering = true,
    };
    VkPhysicalDeviceVulkan12Features features12 {
      .descriptorIndexing = true,
      .bufferDeviceAddress = true,
    };
    
    ///// Loop through physical devices /////
    for (const auto& gpu : gpus) {
      bool is_suitable = true;

      ///// Get GPU properties
      VkPhysicalDeviceProperties properties;
      vkGetPhysicalDeviceProperties(gpu, &properties);
      fmt::println("GPU Name: {}", properties.deviceName);
      // VkPhysicalDeviceFeatures2 features;
      // vkGetPhysicalDeviceFeatures2(gpu, &features);
      is_suitable &= (properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU);

      ///// Enumerate device extensions
      is_suitable &= check_device_extension_support(gpu);

      ///// Check if the swap chain support is adequate
      // is_suitable &= check_swapchain_support(gpu);

      ///// Find queue families
      uint32_t family_count = 0;
      vkGetPhysicalDeviceQueueFamilyProperties(gpu, &family_count, nullptr);
      std::vector<VkQueueFamilyProperties> families(family_count);
      vkGetPhysicalDeviceQueueFamilyProperties(gpu, &family_count, families.data());

      // See if there is a queue family that has graphics queues
      int graphics_family_index = 0;
      bool check_adequate_queue_flags = false;
      for (const auto& family : families) {
        if (check_adequate_queue_flags |= ( family.queueCount &&
            (family.queueFlags & VK_QUEUE_GRAPHICS_BIT) && 
            (family.queueFlags & VK_QUEUE_COMPUTE_BIT) )) break;
        ++graphics_family_index;
      }

      is_suitable &= check_adequate_queue_flags;

      // See if there is a queue family that supports the current surface presentation
      int present_family_index = 0;
      VkBool32 check_present_support = false;
      for (const auto& family : families) {
        vkGetPhysicalDeviceSurfaceSupportKHR(gpu, present_family_index, _surface, &check_present_support);
        if (check_present_support) break;
        ++present_family_index;
      }

      is_suitable &= static_cast<bool>(check_present_support);

      if (is_suitable) suitable_gpus.push_back({gpu, graphics_family_index, present_family_index});
    }
  }; physical_device_selector(suitable_gpus);
 
  if (!suitable_gpus.size()) {
    throw std::runtime_error("Suitable GPUs not found!");
  }

  if (suitable_gpus.size() > 1) {
    // Enable user selection
    fmt::println("More than one suitable GPU found.");
  }
  
  _selectedGPU = suitable_gpus[0]._gpu;  // Select the first GPU for now.
  ///////////////////////////////////////////////////////////////////////

  ///// Set the queue create infos
  std::vector<VkDeviceQueueCreateInfo> queue_create_infos;
  std::set<uint32_t> queue_family_set = {
    suitable_gpus[0]._graphics_family_index.value(),
    suitable_gpus[0]._present_family_index.value(),
  };

  float queue_priority = 1.0f;
  for (uint32_t queue_family_index : queue_family_set) {
    queue_create_infos.push_back({
      .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
      .queueFamilyIndex = queue_family_index,
      .queueCount = 1,
      .pQueuePriorities = &queue_priority,
    }); 
  }

  // ///// Create a graphics queue
  // VkDeviceQueueCreateInfo queue_info = {
  //   .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
  //   .queueFamilyIndex = suitable_gpus[0]._graphics_family_index.value(),
  //   .queueCount = 1,
  //   .pQueuePriorities = &queue_priority,
  // };

  // ///// Create a presentation queue
  // float present_queue_priority = 1.0f;
  // VkDeviceQueueCreateInfo present_queue_info = {
  //   .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
  //   .queueFamilyIndex = suitable_gpus[0]._present_family_index.value(),
  //   .queueCount = 1,
  //   .pQueuePriorities = &queue_priority,
  // };

  ///// Create the logical device
  VkPhysicalDeviceVulkan13Features features13{
    .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES,
    .synchronization2 = true,
    .dynamicRendering = true,
  };
  VkPhysicalDeviceVulkan12Features features12{
    .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES,
    .pNext = &features13,
    .descriptorIndexing = true,
    .bufferDeviceAddress = true,
  };
  VkPhysicalDeviceFeatures2 features2{
    .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2,
    .pNext = &features12,
  };
  
  //If the pNext chain includes a VkPhysicalDeviceFeatures2 structure, 
  // then pEnabledFeatures must be NULL
  // VkPhysicalDeviceFeatures features{};

  VkDeviceCreateInfo device_info = {
    .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
    .pNext = &features2,
    .queueCreateInfoCount = static_cast<uint32_t>(queue_create_infos.size()),
    .pQueueCreateInfos = queue_create_infos.data(),
    .enabledExtensionCount = static_cast<uint32_t>(device_extensions.size()),
    .ppEnabledExtensionNames = device_extensions.data(),
  };
  
  VK_CHECK(vkCreateDevice(_selectedGPU, &device_info, nullptr, &_device));
  
  vkGetDeviceQueue(_device, suitable_gpus[0]._graphics_family_index.value(), 0, &_queue);
  vkGetDeviceQueue(_device, suitable_gpus[0]._present_family_index.value(), 0, &_present_queue);
}

void VulkanEngine::init_swapchain() {
  create_swapchain(_windowExtent.width, _windowExtent.height);
}

void VulkanEngine::init_commands()
{
}

void VulkanEngine::init_sync_structures()
{
}

void VulkanEngine::create_swapchain(uint32_t width, uint32_t height) {
  
  struct _swapchain_support_details {
    VkSurfaceCapabilitiesKHR _capabilities;
    std::vector<VkSurfaceFormatKHR> _formats;
    std::vector<VkPresentModeKHR> _present_modes;
  };
  auto query_swapchain_support = [this]() {
    _swapchain_support_details details;

    // Fetch the basic surface capabilities
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(_selectedGPU, _surface, &details._capabilities);

    // Query the supported surface formats
    uint32_t format_count;
    vkGetPhysicalDeviceSurfaceFormatsKHR(_selectedGPU, _surface, &format_count, nullptr);
    if (format_count != 0) {
      details._formats.resize(format_count);
      vkGetPhysicalDeviceSurfaceFormatsKHR(_selectedGPU, _surface, &format_count, details._formats.data());
    }

    // Query the supported presentation modes
    uint32_t present_mode_count;
    vkGetPhysicalDeviceSurfacePresentModesKHR(_selectedGPU, _surface, &present_mode_count, nullptr);
    if (present_mode_count != 0) {
      details._present_modes.resize(present_mode_count);
      vkGetPhysicalDeviceSurfacePresentModesKHR(_selectedGPU, _surface, &present_mode_count, details._present_modes.data());
    }

    return details;
  };

  auto select_surface_format = [](const std::vector<VkSurfaceFormatKHR>& available_formats) {
    for (const auto& available_format : available_formats) {
      if (available_format.format == VK_FORMAT_B8G8R8A8_SRGB &&
        available_format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
        return available_format;
    }
    return available_formats[0];
  };

  auto select_present_mode = [](const std::vector<VkPresentModeKHR>& available_present_modes) {
    for (const auto& available_present_mode : available_present_modes) {
      if (available_present_mode == VK_PRESENT_MODE_MAILBOX_KHR)
        return available_present_mode;
    }
    return VK_PRESENT_MODE_FIFO_KHR;
  };

  auto select_swap_extent = [width, height](const VkSurfaceCapabilitiesKHR& capabilities) {
    if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
      return capabilities.currentExtent;
    
    // When the currentExtent.width is the max value of uint32_t,
    // the current extent can differ.

    VkExtent2D extent = {
      static_cast<uint32_t>(width),
      static_cast<uint32_t>(height)
    };

    extent.width = std::clamp(extent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
    extent.height = std::clamp(extent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);
  
    return extent;
  };

  ///////////////////////////////////////////////////////////////////////

  _swapchain_support_details detail = query_swapchain_support();

  VkSurfaceFormatKHR surface_format = select_surface_format(detail._formats);
  VkPresentModeKHR present_mode = select_present_mode(detail._present_modes);
  VkExtent2D extent = select_swap_extent(detail._capabilities);

  uint32_t image_count = detail._capabilities.minImageCount + 1;
  if (detail._capabilities.maxImageCount > 0 &&
    image_count > detail._capabilities.maxImageCount) {
    image_count = detail._capabilities.maxImageCount;
  }

  VkSwapchainCreateInfoKHR create_info = {
    .sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
    .surface = _surface,
    .minImageCount = image_count,
    .imageFormat = surface_format.format,
    .imageColorSpace = surface_format.colorSpace,
    .imageExtent = extent,
    .imageArrayLayers = 1,
    .imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,

    // Need to be changed if the graphics queue family index
    // and the present queue family index are not the same
    .imageSharingMode = VK_SHARING_MODE_EXCLUSIVE,
    .queueFamilyIndexCount = 0,
    .pQueueFamilyIndices = nullptr,

    .preTransform = detail._capabilities.currentTransform,

    .compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,

    .presentMode = present_mode,
    .clipped = VK_TRUE,

    .oldSwapchain = VK_NULL_HANDLE
  };

  VK_CHECK(vkCreateSwapchainKHR(_device, &create_info, nullptr, &_swapchain));

  ///// Retrieve the swap chain images
  vkGetSwapchainImagesKHR(_device, _swapchain, &image_count, nullptr);
  _swapchain_images.resize(image_count);
  vkGetSwapchainImagesKHR(_device, _swapchain, &image_count, _swapchain_images.data());
  ///// Retrieve the swap chain image views
  _swapchain_image_views.resize(image_count);

  for (size_t i=0; i<image_count; ++i) {
    VkImageViewCreateInfo create_info = {
      .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
      .image = _swapchain_images[i],
      .viewType = VK_IMAGE_VIEW_TYPE_2D,
      .format = surface_format.format,
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

    VK_CHECK(vkCreateImageView(_device, &create_info, nullptr, &_swapchain_image_views[i]));
  }
  
  _swapchain_image_format = surface_format.format;
  _swapchain_extent = extent;
}

void VulkanEngine::destroy_swapchain() {
  vkDestroySwapchainKHR(_device, _swapchain, nullptr);

  // Destroy image views
  for (auto& _image_view : _swapchain_image_views)
    vkDestroyImageView(_device, _image_view, nullptr);
}

/////////////////////////////////////////////////////
///////// Debug Messenger Setup Functions ///////////
/////////////////////////////////////////////////////
#ifndef NDEBUG  /* Debug Mode On */

VKAPI_ATTR VkBool32 VKAPI_CALL VulkanEngine::debugCallback(
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

inline bool VulkanEngine::checkInstanceExtensionSupport(std::vector<const char*> exts) {
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

inline bool VulkanEngine::checkValidationLayerSupport(std::vector<const char*> layers) {
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

inline void VulkanEngine::populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& debug_messenger_create_info) {
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

inline void VulkanEngine::setupDebugMessenger() {
    
  fmt::print("Debug Mode is On!\n");
  fmt::print("Set up the debug messenger...");

  VkDebugUtilsMessengerCreateInfoEXT debug_messenger_create_info;
  populateDebugMessengerCreateInfo(debug_messenger_create_info);

  VK_CHECK(CreateDebugUtilsMessengerEXT(
    _instance, 
    &debug_messenger_create_info, 
    nullptr, 
    &_debug_messenger
  )); fmt::print("done\n");
}

inline void VulkanEngine::cleanupDebugMessenger() {

  fmt::print("Clean up a debug messenger...");

  DestroyDebugUtilsMessengerEXT(_instance, _debug_messenger, nullptr);
  fmt::print("done\n");
}

#endif  // !NDEBUG