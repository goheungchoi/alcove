// vk_engine.h
#pragma once
#include <core/engine/vk_common.h>

struct FrameData {
  VkCommandPool _command_pool;
  VkCommandBuffer _main_command_buffer;
};

constexpr unsigned int FRAME_OVERLAP = 2;

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
  VkDevice                  _device;          // Vulkan device for commands
  VkQueue                   _graphics_queue;  // Device graphics queue handle
  VkQueue                   _present_queue;   // Device presentation queue handle
  VkSurfaceKHR              _surface;         // Vulkan window surface

private:
  void init_vulkan();
  void init_swapchain();
  void init_commands();
  void init_sync_structures();

public:
  VkSwapchainKHR  _swapchain;
  VkFormat        _swapchain_image_format;

  std::vector<VkImage> _swapchain_images;
  std::vector<VkImageView> _swapchain_image_views;
  VkExtent2D  _swapchain_extent;

private:
  void create_swapchain(uint32_t width, uint32_t height);
  void destroy_swapchain();

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

private:
  /// @brief GPU Selection helper class for the sake of code readibility.
  class GPU_Selector {
    std::multimap<int, VkPhysicalDevice> candidates;
    std::vector<std::string> _device_extensions;
    
    std::optional<VkPhysicalDeviceType> _required_device_type;

    // Features

    // Features 1.1

    // Features 1.2

    // Features 1.3

    // Graphics Queue Family
    bool _check_graphics_queue_family_support = false;
    std::set<VkQueueFlagBits> _queue_flag_bits;
    std::optional<uint32_t> _graphics_family_index;
    // Present Queue Family
    bool _check_present_queue_family_support = false;
    std::optional<uint32_t> _present_family_index;

    public:
      GPU_Selector& required_physical_device_type(VkPhysicalDeviceType device_type) {
        _required_device_type = device_type;
        return *this;
      }

      GPU_Selector& required_features(VkPhysicalDeviceFeatures device_features) {

        return *this;
      }

      GPU_Selector& required_features_1_1(VkPhysicalDeviceVulkan11Features vk11features) {

        return *this;
      }

      GPU_Selector& required_features_1_2(VkPhysicalDeviceVulkan12Features vk12features) {

        return *this;
      }

      GPU_Selector& required_features_1_3(VkPhysicalDeviceVulkan13Features vk13features) {

        return *this;
      }

      template<typename ...Args>
      GPU_Selector& requires_graphics_queue_family(Args... args) {
        static_assert((std::is_same<Args, VkQueueFlagBits>::value && ...),
          "All arguments must be of VkQueueFlagBits."
        );
        _check_graphics_queue_family_support = true;
        (_queue_flag_bits.insert(args), ...)
        return *this;
      }

      GPU_Selector& requires_present_queue_family() {
        _check_present_queue_family_support = true;
        return *this;
      }

      GPU_Selector& requires_device_extensions(const std::vector<const char*>& device_extensions) {
        for (const auto& device_extension : device_extensions) {
          _device_extensions.emplace_back(device_extension);
        }
        return *this;
      }

      VkPhysicalDevice operator() (VkInstance _instance) {
        uint32_t gpus_count = 0;
        VK_CHECK(vkEnumeratePhysicalDevices(_instance, &gpus_count, nullptr));
        if (!gpus_count) {
          throw std::runtime_error("GPUs supporting Vulkan not found!");
        }
        std::vector<VkPhysicalDevice> gpus(gpus_count);
        VK_CHECK(vkEnumeratePhysicalDevices(_instance, &gpus_count, gpus.data()));

        for (const auto& gpu : gpus) {
          int score = 0;

        }

      }

    private:
      size_t check_physical_device_type(VkPhysicalDevice gpu) {
        VkPhysicalDeviceProperties properties;
        vkGetPhysicalDeviceProperties(gpu, &properties);
        fmt::println("GPU Name: {}", properties.deviceName);
        return (properties.deviceType == _required_device_type) * 10;
      }

      size_t check_device_extensions_support(VkPhysicalDevice gpu) {
        uint32_t extension_count;
        vkEnumerateDeviceExtensionProperties(gpu, nullptr, &extension_count, nullptr);

        std::vector<VkExtensionProperties> available_extensions(extension_count);
        vkEnumerateDeviceExtensionProperties(gpu, nullptr, &extension_count, available_extensions.data());

        std::set<std::string> required_extensions(_device_extensions.begin(), _device_extensions.end());
        for (const auto& extension : available_extensions) {
          required_extensions.erase(extension.extensionName);
        }

        return required_extensions.size();
      }

      size_t check_graphics_queue_family(VkPhysicalDevice gpu) {
        uint32_t family_count = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(gpu, &family_count, nullptr);
        std::vector<VkQueueFamilyProperties> families(family_count);
        vkGetPhysicalDeviceQueueFamilyProperties(gpu, &family_count, families.data());

        int max_num_flag_matches = 0;
        int graphics_family_index = 0;
        for (const auto& family : families) {
          int num_flag_matches = 0;
          if (family.queueCount && 
              std::all_of(std::begin(_queue_flag_bits), std::end(_queue_flag_bits),
                [&family, &num_flag_matches](VkQueueFlagBits bit) { 
                  num_flag_matches += (family.queueFlags & bit) && true;
                  return family.queueFlags & bit; 
                }
              )
          ) {
            _graphics_family_index = graphics_family_index;
            return num_flag_matches;
          }
          max_num_flag_matches = std::max(max_num_flag_matches, num_flag_matches);
          ++graphics_family_index;
        }

        return max_num_flag_matches;
      }

      size_t check_present_queue_family(VkPhysicalDevice gpu, VkSurfaceKHR _surface) {
        uint32_t family_count = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(gpu, &family_count, nullptr);
        std::vector<VkQueueFamilyProperties> families(family_count);
        vkGetPhysicalDeviceQueueFamilyProperties(gpu, &family_count, families.data());

        uint32_t present_family_index = 0;
        VkBool32 check_present_support = false;
        for (const auto& family : families) {
          vkGetPhysicalDeviceSurfaceSupportKHR(gpu, present_family_index, _surface, &check_present_support);
          if (check_present_support) {
            _present_family_index = present_family_index;
            return 1;
          }
          ++present_family_index;
        }

        return 0;
      }

    public:
      std::optional<uint32_t> get_graphics_queue_family_index() { 
        return _graphics_family_index; 
      }

      std::optional<uint32_t> get_present_queue_family_index() { 
        return _present_family_index; 
      }
  };
};

