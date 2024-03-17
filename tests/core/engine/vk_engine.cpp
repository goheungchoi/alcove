#include "vk_engine.h"

#include <SDL.h>        // The main SDL library data for opening a window and input
#include <SDL_vulkan.h> // Vulkan-specific flags and functionality for opening a Vulkan-compatible window

#include <vk_initializers.h>
#include <vk_common.h>

#include <chrono>
#include <thread>

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

  // Get instance extensions from SDL library.
  // If the SDL window is not set, throw an error.
  if (!_window) throw std::runtime_error("SDL Window is not created!");
  unsigned int ext_count = 0;
  SDL_Vulkan_GetInstanceExtensions(_window, &ext_count, nullptr);
  std::vector<const char*> exts(ext_count);
  SDL_Vulkan_GetInstanceExtensions(_window, &ext_count, exts.data());

  // Need a validation layer.
  const std::vector<const char*> layers = {
    "VK_LAYER_KHRONOS_validation"
  };

  VkInstanceCreateInfo instance_info = {
    .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
    .pNext = nullptr,
    .flags = 0,
    .pApplicationInfo = &app_info,
    .enabledLayerCount = static_cast<uint32_t>(layers.size()),
    .ppEnabledLayerNames = layers.data(),
    .enabledExtensionCount = static_cast<uint32_t>(exts.size()),
    .ppEnabledExtensionNames = exts.data(),
  };

  VK_CHECK(vkCreateInstance(&instance_info, nullptr, &_instance));

  // TODO: Debug messenger is required
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
