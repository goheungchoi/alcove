// vk_engine.h
#pragma once
#include <core/engine/vk_common.h>
// Includes all the common types to be used throughout the engine.
#include <core/engine/vk_types.h>


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

/* VULKAN SETUP */
public:
  VkInstance                _instance;        // Vulkan library handle
  VkDebugUtilsMessengerEXT  _debug_messenger; // Vulkan debug output handle
  VkPhysicalDevice          _selectedGPU;     // GPU chosen as the default device
  VkDevice                  _device;          // Vulkan device for commands
  QueueData									_graphics_q;			// Device graphics queue struct
  QueueData									_present_q;				// Device presentation queue struct
  VkSurfaceKHR              _surface;         // Vulkan window surface
	
	DeletionQueue							_main_deletion_queue;

private:
  void init_vulkan();
  void init_swapchain();
  void init_commands();
  void init_sync_structures();

public:
  VkSwapchainKHR  _swapchain;
  VkFormat        _swapchain_image_format;

	Canvas _canvas;
  std::vector<VkImage> _swapchain_images;
  std::vector<VkImageView> _swapchain_image_views;
  VkExtent2D  _swapchain_extent;

private:
  void create_swapchain(uint32_t width, uint32_t height);
  void destroy_swapchain();

public:
	static constexpr std::size_t FRAME_OVERLAP{ 2 };

  FrameData _frames[FRAME_OVERLAP];
  FrameData& get_current_frame() { return _frames[_frameNumber % FRAME_OVERLAP]; }

	VmaAllocator _allocator;


#ifndef NDEBUG
public:
  static VKAPI_ATTR VkBool32 VKAPI_CALL  debugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
    void* pUserData
  );
  
private:
  inline bool checkInstanceExtensionSupport(std::vector<const char*> exts);
  inline bool checkValidationLayerSupport(std::vector<const char*> layers);

  inline void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT&);

  inline void setupDebugMessenger();
  inline void cleanupDebugMessenger();

  inline VkResult CreateDebugUtilsMessengerEXT(
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
  
  inline void DestroyDebugUtilsMessengerEXT(
    VkInstance instance, 
    VkDebugUtilsMessengerEXT debugMessenger, 
    const VkAllocationCallbacks* pAllocator
  ) {
    auto func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
    if (func != nullptr) {
      func(instance, debugMessenger, pAllocator);
    }
  }
#endif  // !NDEBUG
};

