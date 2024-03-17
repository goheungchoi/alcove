#pragma once
#include <vk_common.h>

class VulkanEngine {
public:
  bool _isInitialized{ false }; // Check if the engine is initialized
  int _frameNumber{ 0 };        // A frame number integer
  bool stop_rendering{ false }; 
  VkExtent2D _windowExtent{ 1700, 900 };  // The pixel size of the window to be opened

  struct SDL_Window* _window{ nullptr };  // Forward-declared SDL_Window to be attached

  static VulkanEngine& Get();

  // Initializes everything in the engine
  void init();

  // Shuts down the engine
  void cleanup();

  // Draw loop
  void draw();

  // Run the main loop
  void run();

public:
  VkInstance                _instance;        // Vulkan library handle
  VkDebugUtilsMessengerEXT  _debug_messenger; // Vulkan debug output handle
  VkPhysicalDevice          _selectedGPU;     // GPU chosen as the default device
  VkDevice                  _device;          // Vulakn device for commands
  VkSurfaceKHR              _surface;         // Vulkan window surface

private:
  void init_vulkan();
  void init_swapchain();
  void init_commands();
  void init_sync_structures();
};

