// vk_gpu_selector.h
#pragma once
#include <core/engine/vk_common.h>

enum class SelectOption {
  REQUIRED,   // Exclude any GPUs without this feature 
  IMPORTANT,  // Weight this feature more
  OPTIONAL    // Normal weight
};

/// @brief GPU Selection helper class for the sake of code readibility.
class GPU_Selector {
  int EXTRA_WEIGHT{ 0xFF };

  std::multimap<int, VkPhysicalDevice, std::greater<int>> candidates;  // GPU candidates

  /**
   * @brief Store the extension names in string. 
   * VK_KHR_SWAPCHAIN_EXTENSION_NAME for swapchain support.
   * 
   */
  std::vector<std::string> _queried_device_extensions;      // requested device extensions
  SelectOption _queried_device_extensions_select_option{ SelectOption::REQUIRED };

  /**
   * @brief If this variable doesn't indicate a specific device type, 
   * the GPU_Selector will rate device types as follows: 
   *     VK_PHYSICAL_DEVICE_TYPE_OTHER = 1,
   *     VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU = 4,
   *     VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU = 5,
   *     VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU = 3,
   *     VK_PHYSICAL_DEVICE_TYPE_CPU = 2.
   */
  std::optional<VkPhysicalDeviceType> _queried_device_type;
  SelectOption _queried_device_type_select_option{ SelectOption::IMPORTANT };

  // Features
  VkPhysicalDeviceFeatures2 _queried_features{};
  SelectOption _queried_features_select_option{ SelectOption::OPTIONAL };
  // Features 1.1
  VkPhysicalDeviceVulkan11Features _queried_features11{};
  SelectOption _queried_features11_select_option{ SelectOption::OPTIONAL };
  // Features 1.2
  VkPhysicalDeviceVulkan12Features _queried_features12{};
  SelectOption _queried_features12_select_option{ SelectOption::OPTIONAL };
  // Features 1.3
  VkPhysicalDeviceVulkan13Features _queried_features13{};
  SelectOption _queried_features13_select_option{ SelectOption::OPTIONAL };


  // User-defined queue families
  std::unordered_map<std::string, std::optional<uint32_t>> _queue_families;

  // Graphics Queue Family
  bool _check_graphics_queue_family_support = false;
  std::set<VkQueueFlagBits> _queried_graphics_queue_flag_bits;
  std::optional<uint32_t> _graphics_family_index;
  SelectOption _graphics_queue_family_select_option{ SelectOption::REQUIRED };


  // Present Queue Family
  bool _check_present_queue_family_support = false;
  VkSurfaceKHR _present_surface{};
  std::optional<uint32_t> _present_family_index;
  SelectOption _present_queue_family_select_option{ SelectOption::REQUIRED };


  // Selected GPU
  VkPhysicalDevice _selected_GPU;
public:
  GPU_Selector() :
  _queried_features13{
    .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES
  },
  _queried_features12{
    .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES,
    .pNext = &_queried_features13,
  },
  _queried_features11{
    .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_1_FEATURES,
    .pNext = &_queried_features12,
  },
  _queried_features{
    .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2,
    .pNext = &_queried_features11,
  } {}
  
  GPU_Selector& set_increase_rate(int rate) {
    EXTRA_WEIGHT = rate;
  }

  GPU_Selector& query_physical_device_type(
    VkPhysicalDeviceType device_type,
    SelectOption option = SelectOption::IMPORTANT
  ) {
    _queried_device_type = device_type;
    _queried_device_type_select_option = option;
    return *this;
  }

  GPU_Selector& query_device_extensions(
    const std::vector<const char*>& device_extensions) {
    for (const auto& device_extension : device_extensions) {
      _queried_device_extensions.emplace_back(device_extension);
    }
    return *this;
  }

  GPU_Selector& query_features(
    VkPhysicalDeviceFeatures2 device_features,
    SelectOption option = SelectOption::OPTIONAL
  ) {
    _queried_features = device_features;
    _queried_features_select_option = option;
    return *this;
  }

  GPU_Selector& query_features_1_1(
    VkPhysicalDeviceVulkan11Features vk11features,
    SelectOption option = SelectOption::OPTIONAL
  ) {
    _queried_features11 = vk11features;
    _queried_features11_select_option = option;
    return *this;
  }

  GPU_Selector& query_features_1_2(
    VkPhysicalDeviceVulkan12Features vk12features,
    SelectOption option = SelectOption::OPTIONAL
  ) {
    _queried_features12 = vk12features;
    _queried_features12_select_option = option;
    return *this;
  }

  GPU_Selector& query_features_1_3(
    VkPhysicalDeviceVulkan13Features vk13features,
    SelectOption option = SelectOption::OPTIONAL
  ) {
    _queried_features13 = vk13features;
    _queried_features13_select_option = option;
    return *this;
  }

  template<typename ...Args>
  GPU_Selector& query_graphics_queue_family(Args... args) {
    static_assert((std::is_same<Args, VkQueueFlagBits>::value && ...),
      "All arguments must be of VkQueueFlagBits."
    );
    _check_graphics_queue_family_support = true;
    (_queried_graphics_queue_flag_bits.insert(args), ...);
    return *this;
  }

  GPU_Selector& query_present_queue_family(VkSurfaceKHR _surface) {
    _check_present_queue_family_support = true;
    _present_surface = _surface;
    return *this;
  }

  // Execute the selector and return the suitable physical device
  GPU_Selector& select_GPU(VkInstance _instance) {
#ifndef NDEBUG
    fmt::print("Selecting GPU...\n");
#endif

    uint32_t gpus_count = 0;
    VK_CHECK(vkEnumeratePhysicalDevices(_instance, &gpus_count, nullptr));
    if (!gpus_count) {
      throw std::runtime_error("GPUs supporting Vulkan not found!");
    }
    std::vector<VkPhysicalDevice> gpus(gpus_count);
    VK_CHECK(vkEnumeratePhysicalDevices(_instance, &gpus_count, gpus.data()));
    
    // Calculate scores
    for (const auto& gpu : gpus) {
      int score = 0;
      
      if (!check_physical_device_type(gpu, &score)) {
#ifndef NDEBUG
        fmt::println("GPU_Selector: The device type doesn't match!");
        fmt::println("GPU_Selector: Continue.");
#endif
        continue;
      }

      if (!check_device_extensions_support(gpu, &score)) {
#ifndef NDEBUG
        fmt::println("GPU_Selector: Required extensions not supported!");
        fmt::println("GPU_Selector: Continue.");
#endif
        continue;
      }

      if (!check_graphics_queue_family(gpu, &score)) {
#ifndef NDEBUG
        fmt::println("GPU_Selector: Graphics queue families not found!");
        fmt::println("GPU_Selector: Continue.");
#endif
        continue;
      }

      if (!check_present_queue_family(gpu, &score)) {
#ifndef NDEBUG
        fmt::println("GPU_Selector: Presnet queue families not found!");
        fmt::println("GPU_Selector: Continue.");
#endif
        continue;
      }

			// TODO: Need to check support for features.

      candidates.insert({score, gpu});
    }

    if (candidates.empty()) {
      fmt::println("No suitable GPUs are found!");
      fmt::println("abort()");
      abort();
    }

    VkPhysicalDevice selected_GPU = candidates.begin()->second;

    if (_check_graphics_queue_family_support) {
      _graphics_family_index = get_graphics_queue_family_index(selected_GPU);
    }
    if (_check_present_queue_family_support) {
      _present_family_index = get_present_queue_family_index(selected_GPU);
    }

    _selected_GPU = selected_GPU;

#ifndef NDEBUG
    fmt::print("done\n");
#endif

		return *this;
  }

private:
  bool check_physical_device_type(VkPhysicalDevice gpu, int* score) {
    VkPhysicalDeviceProperties properties;
    vkGetPhysicalDeviceProperties(gpu, &properties);
#ifndef NDEBUG
    fmt::println("GPU Name: {}", properties.deviceName);
    fmt::println("Supported Vulkan API Version: {}.{}.{}", 
      VK_API_VERSION_MAJOR(properties.apiVersion),
      VK_API_VERSION_MINOR(properties.apiVersion),
      VK_API_VERSION_PATCH(properties.apiVersion)
    );
#endif

    int type_score = 0;
    if (_queried_device_type) { // User requested device type
      type_score = (properties.deviceType == _queried_device_type);
    } else {  // No specific device type is not requested.
#ifndef NDEBUG
      fmt::println("GPU_Selector: Device type is not specified!");
      fmt::println("GPU_Selector: The default device type rating will be used.");
#endif
      // Default device type scores
      switch (properties.deviceType) {
        case VkPhysicalDeviceType::VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU:
          type_score = 5;
          break;
        case VkPhysicalDeviceType::VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU:
          type_score = 4;
          break;
        case VkPhysicalDeviceType::VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU:
          type_score = 3;
          break;
        case VkPhysicalDeviceType::VK_PHYSICAL_DEVICE_TYPE_CPU:
          type_score = 2;
          break;
        case VkPhysicalDeviceType::VK_PHYSICAL_DEVICE_TYPE_OTHER:
          type_score = 1;
          break;
      }
    }

    switch (_queried_device_type_select_option) {
      case SelectOption::REQUIRED: {
        *score += type_score * EXTRA_WEIGHT;
        return properties.deviceType == _queried_device_type;
      } break;
      
      case SelectOption::IMPORTANT: {
        *score += type_score * EXTRA_WEIGHT;
        return true;
      } break;

      case SelectOption::OPTIONAL: {
        *score += type_score;
        return true;
      } break;
    }

    return false;
  }

  bool check_device_extensions_support(VkPhysicalDevice gpu, int* score) {
    uint32_t extension_count;
    vkEnumerateDeviceExtensionProperties(gpu, nullptr, &extension_count, nullptr);
    std::vector<VkExtensionProperties> available_extensions(extension_count);
    vkEnumerateDeviceExtensionProperties(gpu, nullptr, &extension_count, available_extensions.data());

    std::set<std::string> unsupported_extensions(_queried_device_extensions.begin(), _queried_device_extensions.end());
    for (const auto& extension : available_extensions) {
      // Check out available extensions
      unsupported_extensions.erase(extension.extensionName);
    }

    int extension_score = _queried_device_extensions.size() - unsupported_extensions.size();
    switch (_queried_device_extensions_select_option) {
      case SelectOption::REQUIRED: {
        *score += extension_score * EXTRA_WEIGHT;
        return unsupported_extensions.empty();
      } break;
      
      case SelectOption::IMPORTANT: {
        *score += extension_score * EXTRA_WEIGHT;
        return true;
      } break;

      case SelectOption::OPTIONAL: {
        *score += extension_score;
        return true;
      } break;
    }

    return false;
  }

  bool check_graphics_queue_family(VkPhysicalDevice gpu, int* score) {
    if (!_check_graphics_queue_family_support) return true;

    // The graphics queue must have the `VK_QUEUE_GRAPHICS_BIT` flag bit.
    _queried_graphics_queue_flag_bits.insert(VK_QUEUE_GRAPHICS_BIT);

    // Get the queue families supported by the GPU.
    uint32_t family_count = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(gpu, &family_count, nullptr);
    std::vector<VkQueueFamilyProperties> families(family_count);
    vkGetPhysicalDeviceQueueFamilyProperties(gpu, &family_count, families.data());

    // pairs of the number of flag matches and the index of the queue family
    std::multimap<int, std::size_t, std::greater<int>> queue_family_candidates;

    std::size_t graphics_family_index = 0;
    for (const auto& family : families) {
      if (!family.queueCount) continue;

      int num_flag_matches = 0;

      for (VkQueueFlagBits bit : _queried_graphics_queue_flag_bits) {
        VkQueueFlags match = family.queueFlags & bit;
        num_flag_matches += match && true;
      }

      if (num_flag_matches) {
        queue_family_candidates.insert({num_flag_matches, graphics_family_index});
      }

      ++graphics_family_index;
    }

    // If the device doesn't support any graphics queue families.
    if (queue_family_candidates.empty()) return false;

    // Get the max number of flag matches
    int candidate_num_flag_matches = queue_family_candidates.begin()->first;
    // Get the most suitable candidate index
    std::size_t candidate_index = queue_family_candidates.begin()->second;

    int optimal_num_flag_matches = _queried_graphics_queue_flag_bits.size();
    int graphics_queue_family_score = candidate_num_flag_matches;
    switch (_graphics_queue_family_select_option) {
      case SelectOption::REQUIRED: {
        *score += graphics_queue_family_score * EXTRA_WEIGHT;
        return candidate_num_flag_matches == optimal_num_flag_matches;
      } break;
      
      case SelectOption::IMPORTANT: {
        *score += graphics_queue_family_score * EXTRA_WEIGHT;
        return true;
      } break;

      case SelectOption::OPTIONAL: {
        *score += graphics_queue_family_score;
        return true;
      } break;
    }

    return false;
  }

  bool check_present_queue_family(VkPhysicalDevice gpu, int* score) {
    if (!_check_present_queue_family_support) return true;
    
    uint32_t family_count = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(gpu, &family_count, nullptr);
    std::vector<VkQueueFamilyProperties> families(family_count);
    vkGetPhysicalDeviceQueueFamilyProperties(gpu, &family_count, families.data());

    // pairs of the number of flag matches and the index of the queue family
    std::set<std::size_t> queue_family_candidates;

    std::size_t present_family_index = 0;
    for (const auto& family : families) {
      VkBool32 check_present_support = false;
      vkGetPhysicalDeviceSurfaceSupportKHR(gpu, present_family_index, _present_surface, &check_present_support);
      if (check_present_support) {
        queue_family_candidates.insert(present_family_index);
      }
      ++present_family_index;
    }

    int present_queue_family_score = !queue_family_candidates.empty();
    switch (_queried_device_extensions_select_option) {
      case SelectOption::REQUIRED: {
        *score += present_queue_family_score * EXTRA_WEIGHT;
        return present_queue_family_score;
      } break;
      
      case SelectOption::IMPORTANT: {
        *score += present_queue_family_score * EXTRA_WEIGHT;
        return true;
      } break;

      case SelectOption::OPTIONAL: {
        *score += present_queue_family_score;
        return true;
      } break;
    }

    return false;
  }

  bool check_queue_families(VkPhysicalDevice gpu, VkSurfaceKHR _surface) {
    
  }

  std::size_t get_graphics_queue_family_index(VkPhysicalDevice gpu) {
    if (!_check_graphics_queue_family_support) return true;

    // The graphics queue must have the `VK_QUEUE_GRAPHICS_BIT` flag bit.
    _queried_graphics_queue_flag_bits.insert(VK_QUEUE_GRAPHICS_BIT);

    // Get the queue families supported by the GPU.
    uint32_t family_count = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(gpu, &family_count, nullptr);
    std::vector<VkQueueFamilyProperties> families(family_count);
    vkGetPhysicalDeviceQueueFamilyProperties(gpu, &family_count, families.data());

    // pairs of the number of flag matches and the index of the queue family
    std::multimap<int, std::size_t, std::greater<int>> queue_family_candidates;

    std::size_t graphics_family_index = 0;
    for (const auto& family : families) {
      if (!family.queueCount) continue;

      int num_flag_matches = 0;

      for (VkQueueFlagBits bit : _queried_graphics_queue_flag_bits) {
        VkQueueFlags match = family.queueFlags & bit;
        num_flag_matches += match && true;
      }

      if (num_flag_matches) {
        queue_family_candidates.insert({num_flag_matches, graphics_family_index});
      }

      ++graphics_family_index;
    }

    return queue_family_candidates.begin()->second;
  }

  std::size_t get_present_queue_family_index(VkPhysicalDevice gpu) {
    if (!_check_present_queue_family_support) return true;
    
    uint32_t family_count = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(gpu, &family_count, nullptr);
    std::vector<VkQueueFamilyProperties> families(family_count);
    vkGetPhysicalDeviceQueueFamilyProperties(gpu, &family_count, families.data());

    // pairs of the number of flag matches and the index of the queue family
    std::set<std::size_t> queue_family_candidates;

    std::size_t present_family_index = 0;
    for (const auto& family : families) {
      VkBool32 check_present_support = false;
      vkGetPhysicalDeviceSurfaceSupportKHR(gpu, present_family_index, _present_surface, &check_present_support);
      if (check_present_support) {
        queue_family_candidates.insert(present_family_index);
      }
      ++present_family_index;
    }

    return *queue_family_candidates.begin();
  }

public: // Getters
  
  VkPhysicalDevice& get_selected_GPU() {
    return _selected_GPU;
  }

  std::optional<uint32_t> get_graphics_queue_family_index() { 
    return _graphics_family_index; 
  }

  std::optional<uint32_t> get_present_queue_family_index() { 
    return _present_family_index; 
  }
};
