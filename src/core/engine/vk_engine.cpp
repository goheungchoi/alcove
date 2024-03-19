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
#include <core/engine/debug_utils/vk_debug_utils.h>
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
    "Vulkan Engine",
    SDL_WINDOWPOS_UNDEFINED,
    SDL_WINDOWPOS_UNDEFINED,
    _windowExtent.width,
    _windowExtent.height,
    window_flags
  );

  ///// Initialize Vulkan
  init_vulkan();

  init_swapchain();

  init_commands();

  init_sync_structures();


  // Notify that every initialization step went fine.
  _isInitialized = true;
}


void VulkanEngine::cleanup() {
  if (_isInitialized) {
/*** DEBUG: Destroy the debug messenger ***/
if constexpr (debug_t::value) DebugUtils::cleanupDebugMessenger(_instance);
/************************************************************************/
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
if (debug_t::value && !DebugUtils::checkInstanceExtensionSupport(exts)) {
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
if (debug_t::value && !DebugUtils::checkValidationLayerSupport(layers)) {
  throw std::runtime_error("Validation layers requested, but not available!");
}
/********************************************************************/



  ///// Set the instance create info
  VkDebugUtilsMessengerCreateInfoEXT debug_messenger_create_info;
  VkInstanceCreateInfo instance_info = {
    .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
    .pNext = [&debug_messenger_create_info] { // Compile-time assignment
        if constexpr (debug_t::value) { /*** DEBUG: ***/
          DebugUtils::populateDebugMessengerCreateInfo(debug_messenger_create_info);
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
if constexpr (debug_t::value) DebugUtils::setupDebugMessenger(_instance);
/********************************************************************/
}

void VulkanEngine::init_swapchain()
{
}

void VulkanEngine::init_commands()
{
}

void VulkanEngine::init_sync_structures()
{
}
