#include "renderer/vulkan/context/vk_render_device.h"

#include <SDL.h>        // The main SDL library data for opening a window and input
#include <SDL_vulkan.h> // Vulkan-specific flags and functionality for opening a Vulkan-compatible window

#include "renderer/vulkan/context/vk_structs.h"

void VkRenderDevice::Init_Win32(void *hInstance, void *hwnd)
{
}

void VkRenderDevice::Init_SDL2(void *sdlWindow) {
  assert(sdlWindow != nullptr);

  _window = (SDL_Window*) sdlWindow;

  _instance.Init_SDL2(_window);

  _instance.CreateSurface_SDL2(_window, &_surface);

  _instance.FindPhysicalDevice(_surface, _deviceExtensions, &_gpu);
}

void VkRenderDevice::Cleanup()
{
}

void VkRenderDevice::CreateDeviceAndQueues()
{
  int graphicsQueueIndex = GetGraphicsQueueFamilyIndex(_gpu);
  if (graphicsQueueIndex < 0) 
    throw std::exception("Graphics queue not found!");

  int presentQueueIndex = GetPresentQueueFamilyIndex(_gpu, _surface);
  if (presentQueueIndex < 0)
    throw std::exception("Present queue not found!");

  std::set<uint32_t> queueFamilySet = {
		(uint32_t) graphicsQueueIndex,
		(uint32_t) presentQueueIndex,
  };

  // Set the queue create infos
  std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
  float queuePriority = 1.0f;
  for (uint32_t queueFamilyIndex : queueFamilySet) {
    queueCreateInfos.push_back({
      .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
      .queueFamilyIndex = queueFamilyIndex,
      .queueCount = 1,
      .pQueuePriorities = &queuePriority,
    }); 
  }

  ///// Create the logical device ////////////////////////////////////////
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
    .queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size()),
    .pQueueCreateInfos = queueCreateInfos.data(),
    .enabledExtensionCount = static_cast<uint32_t>(_deviceExtensions.size()),
    .ppEnabledExtensionNames = _deviceExtensions.data(),
  };
  
  VK_CHECK(vkCreateDevice(_gpu, &device_info, nullptr, &_device));
  
  _graphicsQueue.index = (uint32_t) graphicsQueueIndex;
  _presentQueue.index = (uint32_t) presentQueueIndex;
  vkGetDeviceQueue(_device, _graphicsQueue.index, 0, &_graphicsQueue.handle);
  vkGetDeviceQueue(_device, _presentQueue.index, 0, &_presentQueue.handle);
}

void VkRenderDevice::DestroyDeviceAndQueues()
{
}

int VkRenderDevice::GetGraphicsQueueFamilyIndex(VkPhysicalDevice gpu) {
  // The graphics queue must have the `VK_QUEUE_GRAPHICS_BIT` flag bit.
  std::set<VkQueueFlagBits> graphicsQueueFlagBits{
    VK_QUEUE_GRAPHICS_BIT, 
    VK_QUEUE_TRANSFER_BIT, 
    VK_QUEUE_COMPUTE_BIT
  };

  // Get the queue families supported by the GPU.
  uint32_t family_count = 0;
  vkGetPhysicalDeviceQueueFamilyProperties(gpu, &family_count, nullptr);
  std::vector<VkQueueFamilyProperties> families(family_count);
  vkGetPhysicalDeviceQueueFamilyProperties(gpu, &family_count, families.data());

  // pairs of the number of flag matches and the index of the queue family
  std::multimap<int, int, std::greater<int>> queueFamilyCandidates;

  int graphicsFamilyIndex{ 0 };
  for (const auto& family : families) {
    if (!family.queueCount) continue;

    int numFlagMatches{ 0 };

    for (VkQueueFlagBits bit : graphicsQueueFlagBits) {
      VkQueueFlags match = family.queueFlags & bit;
      numFlagMatches += match && true;
    }

    if (numFlagMatches) {
      queueFamilyCandidates.insert({ numFlagMatches, graphicsFamilyIndex });
    }

    ++graphicsFamilyIndex;
  }

  if (queueFamilyCandidates.empty()) return -1;

  return queueFamilyCandidates.begin()->second;
}

int VkRenderDevice::GetPresentQueueFamilyIndex(VkPhysicalDevice gpu, VkSurfaceKHR surface) {
  uint32_t familyCount{ 0 };
  vkGetPhysicalDeviceQueueFamilyProperties(gpu, &familyCount, nullptr);
  std::vector<VkQueueFamilyProperties> families(familyCount);
  vkGetPhysicalDeviceQueueFamilyProperties(gpu, &familyCount, families.data());

  // pairs of the number of flag matches and the index of the queue family
  std::set<int> queueFamilyCandidates;

  int presentFamilyIndex{ 0 };
  for (const auto& family : families) {
    VkBool32 bCheckPresentSupport = false;
    vkGetPhysicalDeviceSurfaceSupportKHR(gpu, presentFamilyIndex, surface, &bCheckPresentSupport);
    if (bCheckPresentSupport) {
      queueFamilyCandidates.insert(presentFamilyIndex);
    }
    ++presentFamilyIndex;
  }

  if (queueFamilyCandidates.empty()) return -1;

  return *queueFamilyCandidates.begin();
}

void VkRenderDevice::CreateVmaAllocator() {
  // Initialize the vk memory allocator
	VmaAllocatorCreateInfo allocatorInfo{
		.flags = VMA_ALLOCATOR_CREATE_BUFFER_DEVICE_ADDRESS_BIT,
		.physicalDevice = _gpu,
		.device = _device,
		.instance = _instance,
	};
	vmaCreateAllocator(&allocatorInfo, &_allocator);
}

void VkRenderDevice::DestroyVmaAllocator() {
  vmaDestroyAllocator(_allocator);
}
