// vk_engine.cpp
#include "core/engine/vk_engine.h"

#include <SDL.h>        // The main SDL library data for opening a window and input
#include <SDL_vulkan.h> // Vulkan-specific flags and functionality for opening a Vulkan-compatible window

#include "imgui.h"
#include "../res/bindings/imgui_impl_sdl2.h"
#include "../res/bindings/imgui_impl_vulkan.h"

#include <core/engine/vk_gpu_selector.h>
#include <core/engine/vk_structs.h>
#include <core/engine/vk_images.h>
#include <core/engine/vk_pipelines.h>

#define VMA_IMPLEMENTATION	// Activate the VMA implementation.
#include <vk_mem_alloc.h>

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

    init_descriptors();

    init_pipelines();

    init_imgui();

  } catch (const std::exception& e) {
    fmt::println("{}", e.what());
  }
  
  // Notify that every initialization step went fine.
  _isInitialized = true;
}


void VulkanEngine::cleanup() {
  if (_isInitialized) {
    // Make sure the GPU has stopped
    vkDeviceWaitIdle(_device);

    for (int i=0; i<FRAME_OVERLAP; i++) {
      vkDestroyCommandPool(_device, _frames[i]._command_pool, nullptr);
    
      // Destroy sync objects
      vkDestroyFence(_device, _frames[i]._render_fence, nullptr);
      vkDestroySemaphore(_device, _frames[i]._render_semaphore, nullptr);
      vkDestroySemaphore(_device, _frames[i]._swapchain_semaphore, nullptr);
    
			_frames[i]._local_deletion_queue.flush();
		}

		// Flush the global deletion queue
		_main_deletion_queue.flush();

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
  // Wait until the GPU has finished rendering the last frame.
  // Timeout of 1 second
  VK_CHECK(
    vkWaitForFences(
      _device, 
      1, 
      &get_current_frame()._render_fence, 
      true, 
      1'000'000'000 // ns
    )
  );

	get_current_frame()._local_deletion_queue.flush();

  // After the fence is terminated,
  // reset the fence to re-use in the next frame
  VK_CHECK(
    vkResetFences(
      _device,
      1,
      &get_current_frame()._render_fence
    )
  );

  // Request an image from the swapchain.
  uint32_t swapchainImageIndex;
  VK_CHECK(
    vkAcquireNextImageKHR(
      _device, 
      _swapchain, 
      1'000'000'000, 
      get_current_frame()._swapchain_semaphore,  // Signal when successfully acquires an image
      nullptr, 
      &swapchainImageIndex
    )
  );

  /* Begin Recording Commands */

  // Obtain the current command buffer
  VkCommandBuffer cmdBuf = get_current_frame()._main_command_buffer;

  // The commands finished executing, 
  // so it can be safely reset to begin recording again.
  VK_CHECK(vkResetCommandBuffer(cmdBuf, 0));

  // Set the canvas 2D extent
  _canvas_extent.width = _canvas._extent.width;
  _canvas_extent.height = _canvas._extent.height;

  // Begin the command buffer recording
  VkCommandBufferBeginInfo cmdBufBeginInfo {
    .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
    .pNext = nullptr,

    // This command buffer is for one time use
    .flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT,
    // No inheritance command buffer
    .pInheritanceInfo = nullptr,
  };
  VK_CHECK(vkBeginCommandBuffer(cmdBuf, &cmdBufBeginInfo));
  
  // COMMAND:
  // Transition the swapchain image into a drawable layout
  // (If the image's layout is either of the initial layouts,
  // VK_IMAGE_LAYOUT_UNDEFINED or VK_IMAGE_LAYOUT_PREINITIALIZED,
  // any image subresources must be transitioned to another layout
  // before they are accessed by the device.)
  vkutil::cmd_transition_image(
    cmdBuf,
    _canvas._image,
    VK_IMAGE_LAYOUT_UNDEFINED,  // Don't care the current image layout
    VK_IMAGE_LAYOUT_GENERAL     // Transition to general purpose layout
                                // It supports all types of device access
  );

  // // NOTE: Now, the compute shader takes the role of the drawing operation.
  // // TODO: Temp gradient clear color.
  // VkClearColorValue clearColorValue;
  // float flash = std::abs(std::sin(_frameNumber / 120.f));
  // clearColorValue = {{0.0f, 0.0f, flash, 1.0f}};
  // auto clearRange = vkutil::image_subresource_range(
  //   VK_IMAGE_ASPECT_COLOR_BIT
  // );

  // // COMMAND: Clear the image
  // vkCmdClearColorImage(
  //   cmdBuf, 
  //   _canvas._image,
  //   VK_IMAGE_LAYOUT_GENERAL,
  //   &clearColorValue,
  //   1,
  //   &clearRange
  // );

  // Compute shader drawing

  // COMMAND: Bind the gradient drawing compute pipeline
  vkCmdBindPipeline(cmdBuf, VK_PIPELINE_BIND_POINT_COMPUTE, _gradient_pipeline);

  // COMMAND: Bind the descriptor set containing the draw image for the compute pipeline
  vkCmdBindDescriptorSets(
    cmdBuf, 
    VK_PIPELINE_BIND_POINT_COMPUTE, 
    _gradient_pipeline_layout,  // Pipeline layout
    0,                          // The first set index
    1,                          // The number of descriptor sets to be bound
    &_draw_image_descriptors,   // The descriptor sets
    0,                          // The number of dynamic offsets(?)
    nullptr                     // The dynamic offsets
  );

  // COMMAND: Execute the compute pipeline dispatch.
  //  We are using 16x16 workgroup size so we need to divide by it
  vkCmdDispatch(
    cmdBuf, 
    std::ceil(_canvas_extent.width / 16.0),   // Group count X
    std::ceil(_canvas_extent.height / 16.0),  // Group count Y
    1                                         // Group count Z
  );

  // COMMAND: Transition the canvas into transfer source optimal layout
  vkutil::cmd_transition_image(
    cmdBuf,
    _canvas._image,
    VK_IMAGE_LAYOUT_GENERAL,
    VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL
  );
  
  // COMMAND: Transition the swapchain image into transfer destination optimal layout
  vkutil::cmd_transition_image(
    cmdBuf,
    _swapchain_images[swapchainImageIndex],
    VK_IMAGE_LAYOUT_UNDEFINED,
    VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL 
  );

  // COMMAND: Copy from the canvas to the swapchain
  vkutil::copy_image(
    cmdBuf,
    _canvas._image,
    _canvas_extent,
    _swapchain_images[swapchainImageIndex],
    _swapchain_extent
  );

  // COMMAND: Draw imgui into the swapchain image
  draw_imgui(cmdBuf, _swapchain_image_views[swapchainImageIndex]);

  // COMMAND: Transition the swapchain image into Present
  vkutil::cmd_transition_image(
    cmdBuf,
    _swapchain_images[swapchainImageIndex],
    VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
    VK_IMAGE_LAYOUT_PRESENT_SRC_KHR // The only image layout the swapchain allows for presenting to screen
  );

  // Finish recording commands
  VK_CHECK(vkEndCommandBuffer(cmdBuf));

  // The syncronization structures need to be connected
  // in order to interact correctly with the swapchain.

  // 
  VkSemaphoreSubmitInfo waitSemaphoreSubmitInfo {
    .sType = VK_STRUCTURE_TYPE_SEMAPHORE_SUBMIT_INFO,
    .pNext = nullptr,
    .semaphore = get_current_frame()._swapchain_semaphore,
    .value = 1,         // Used for timeline semaphores
    .stageMask = VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT_KHR,
    .deviceIndex = 0,   // Used for multi-GPU semaphore usage
  };

  VkSemaphoreSubmitInfo signalSemaphoreSubmitInfo {
    .sType = VK_STRUCTURE_TYPE_SEMAPHORE_SUBMIT_INFO,
    .pNext = nullptr,
    .semaphore = get_current_frame()._render_semaphore,
    .value = 1,         // Used for timeline semaphores
    .stageMask = VK_PIPELINE_STAGE_2_ALL_GRAPHICS_BIT,
    .deviceIndex = 0,   // Used for multi-GPU semaphore usage
  };

  VkCommandBufferSubmitInfo cmdBufSubmitInfo {
    .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_SUBMIT_INFO,
    .pNext = nullptr,
    .commandBuffer = cmdBuf,
    .deviceMask = 0   // Used when multi-devices need to execute the command
  };

  // Wrap semaphores and command buffer
  VkSubmitInfo2 submitInfo2 {
    .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO_2,
    .pNext = nullptr,

    // The semaphore to be waited before the execution of the commands
    .waitSemaphoreInfoCount = 1,
    .pWaitSemaphoreInfos = &waitSemaphoreSubmitInfo,

    // Command buffer to be submitted
    .commandBufferInfoCount = 1,
    .pCommandBufferInfos = &cmdBufSubmitInfo,

    // The semaphore to be signaled after the execution of the commands
    .signalSemaphoreInfoCount = 1,
    .pSignalSemaphoreInfos = &signalSemaphoreSubmitInfo,
  };

  // Submit to the queue
  VK_CHECK(
    vkQueueSubmit2(
      _graphics_q._handle,
      1,
      &submitInfo2,
      get_current_frame()._render_fence
    )
  );


  // Prepare presentation
  // Need to make sure the image won't be presented
  // until it has finished the rendering commands.
  VkPresentInfoKHR presentInfo {
    .sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
    .pNext = nullptr,

    // Wait on the _renderSemaphore to be signaled
    .waitSemaphoreCount = 1,
    .pWaitSemaphores = &get_current_frame()._render_semaphore,

    .swapchainCount = 1,
    .pSwapchains = &_swapchain,
    
    .pImageIndices = &swapchainImageIndex,
  };

  VK_CHECK(
    vkQueuePresentKHR(
			_present_q._handle,
      &presentInfo
    )
  );

  // Increase the number of frames drawn
  _frameNumber++;
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

      //send SDL event to imgui for handling
      ImGui_ImplSDL2_ProcessEvent(&e);
    }

    // Update the state

    // Draw the current frame
    //// do not draw if we are minimized
    if (stop_rendering) {
      // Throttle the speed to avoid the endless spinning
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
      continue;
    }

    // imgui new frame
    ImGui_ImplVulkan_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    // Some imgui UI to test
    ImGui::ShowDemoWindow();

    // Make imgui calculate internal draw structures
    ImGui::Render();

    // My draw function
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



  ///// Set the instance create info ////////////////////////////////////////
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

  ///// Create the instance //////////////////////////////////////////////////
  VK_CHECK(vkCreateInstance(&instance_info, nullptr, &_instance));



/*** DEBUG: Set up a debug messenger when debug mode. **************/ 
if constexpr (debug_t::value) setupDebugMessenger();
/********************************************************************/



  ///// Create a VkSurfaceKHR object from the SDL window ///////////////
  // The actual window we will be rendering to
  if (!SDL_Vulkan_CreateSurface(_window, _instance, &_surface)) {
    throw std::runtime_error("Failed to create a SDL window surface!");
  }

  ///// Select a GPU //////////////////////////////////////////////////
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
  GPU_Selector gpu_selector;
  _selectedGPU = gpu_selector
    .query_physical_device_type(VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
    .query_device_extensions(device_extensions)
    ///// Required vulkan version features
    ///// Check if the swap chain support is adequate
    // is_suitable &= check_swapchain_support(gpu);
    .query_graphics_queue_family(
      VK_QUEUE_GRAPHICS_BIT, 
      VK_QUEUE_TRANSFER_BIT, 
      VK_QUEUE_COMPUTE_BIT
    )
    .query_present_queue_family(_surface)
    .select_GPU(_instance)
    .get_selected_GPU();
  ///////////////////////////////////////////////////////////////////////

#ifndef NDEBUG
    fmt::print("Creating a graphics queue and present queue...");
#endif

  // Retrieve Queue family indices
	_graphics_q._index = gpu_selector.get_graphics_queue_family_index().value();
	_present_q._index = gpu_selector.get_present_queue_family_index().value();

  ///// Set the queue create infos ////////////////////////////////////////
  std::vector<VkDeviceQueueCreateInfo> queue_create_infos;
  std::set<uint32_t> queue_family_set = {
		_graphics_q._index,
		_graphics_q._index,
  };

  ///// Create a graphics queue & presentation queue ////////////////////
  float queue_priority = 1.0f;
  for (uint32_t queue_family_index : queue_family_set) {
    queue_create_infos.push_back({
      .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
      .queueFamilyIndex = queue_family_index,
      .queueCount = 1,
      .pQueuePriorities = &queue_priority,
    }); 
  }

#ifndef NDEBUG
    fmt::print("done\n");
#endif

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
    .queueCreateInfoCount = static_cast<uint32_t>(queue_create_infos.size()),
    .pQueueCreateInfos = queue_create_infos.data(),
    .enabledExtensionCount = static_cast<uint32_t>(device_extensions.size()),
    .ppEnabledExtensionNames = device_extensions.data(),
  };
  
  VK_CHECK(vkCreateDevice(_selectedGPU, &device_info, nullptr, &_device));
  
  vkGetDeviceQueue(_device, _graphics_q._index, 0, &_graphics_q._handle);
  vkGetDeviceQueue(_device, _present_q._index, 0, &_present_q._handle);

	///// Initialize the vk memory allocator ////////////////////////////////
	VmaAllocatorCreateInfo allocatorInfo{
		.flags = VMA_ALLOCATOR_CREATE_BUFFER_DEVICE_ADDRESS_BIT,
		.physicalDevice = _selectedGPU,
		.device = _device,
		.instance = _instance,
	};
	vmaCreateAllocator(&allocatorInfo, &_allocator);

	// Reserve a deletion call for the memory allocator.
	_main_deletion_queue.push_function([this]() {
		vmaDestroyAllocator(_allocator);
	});
}

void VulkanEngine::init_swapchain() {
  create_swapchain(_windowExtent.width, _windowExtent.height);

  // Create Canvas
  // Canvas will take care of drawing frames and 
  // transfer it into the swapchain image.
  
  // Image extent to be passed
  VkExtent3D imageExtent {
    _windowExtent.width,
    _windowExtent.height,
    1
  };

  // Image usage flags to be passed
  VkImageUsageFlags imageUsageFlags{};
  imageUsageFlags = 
    VK_IMAGE_USAGE_TRANSFER_SRC_BIT |
    VK_IMAGE_USAGE_TRANSFER_DST_BIT |
    VK_IMAGE_USAGE_STORAGE_BIT |
    VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

  // Image create info
  VkImageCreateInfo canvasInfo {
    .sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
    .pNext = nullptr,

    .imageType = VK_IMAGE_TYPE_2D,

    .format = VK_FORMAT_R16G16B16A16_SFLOAT,
    .extent = imageExtent,

    .mipLevels = 1,
    .arrayLayers = 1,

    .samples = VK_SAMPLE_COUNT_1_BIT, // 1 sample per pixel

    .tiling = VK_IMAGE_TILING_OPTIMAL, // image is stored on the best gpu format
    .usage = imageUsageFlags
  };

  // Allocate the image from gpu local memory
  VmaAllocationCreateInfo imageAllocInfo {
    .usage = VMA_MEMORY_USAGE_GPU_ONLY,
    .requiredFlags = VkMemoryPropertyFlags(VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT)
  };

  // Allocate gpu memory and create canvas
  vmaCreateImage(
    _allocator, 
    &canvasInfo, 
    &imageAllocInfo, 
    &_canvas._image,
    &_canvas._allocation,
    nullptr
  );
  _canvas._extent = imageExtent;
  _canvas._image_format = VK_FORMAT_R16G16B16A16_SFLOAT;

  // Build a image view for the canvas to use for rendering
  VkImageViewCreateInfo canvasViewInfo {
    .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
    .pNext = nullptr,

    .image = _canvas._image,
    .viewType = VK_IMAGE_VIEW_TYPE_2D,
    .format = VK_FORMAT_R16G16B16A16_SFLOAT,
    .subresourceRange = {
      .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
      .baseMipLevel = 0,
      .levelCount = 1,
      .baseArrayLayer = 0,
      .layerCount = 1
    }
  };

  // Create the canvas view
  VK_CHECK(
    vkCreateImageView(
      _device, 
      &canvasViewInfo,
      nullptr,
      &_canvas._image_view
    )
  );
  
  // Reserve the image destroy call.
  _main_deletion_queue.push_function([this]() {
    vkDestroyImageView(_device, _canvas._image_view, nullptr);
    vmaDestroyImage(_allocator, _canvas._image, _canvas._allocation);
  });
}

void VulkanEngine::init_commands() {
  VkCommandPoolCreateInfo commandPoolInfo {
    .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
    .pNext = nullptr,
    .flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
    .queueFamilyIndex = _graphics_q._index
  };

  for (int i = 0; i < FRAME_OVERLAP; ++i) {
    // Create a command pool
    VK_CHECK(vkCreateCommandPool(_device, &commandPoolInfo, nullptr, &_frames[i]._command_pool));

    // Create a main command buffer
    VkCommandBufferAllocateInfo cmdAllocInfo {
      .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
      .pNext = nullptr,
      .commandPool = _frames[i]._command_pool,
      .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
      .commandBufferCount = 1
    };
    VK_CHECK(vkAllocateCommandBuffers(_device, &cmdAllocInfo, &_frames[i]._main_command_buffer));
  }

  /* IMMEDIATE SUBMIT COMMAND SETUP */
  // Create the immediate submit command pool and buffer
  VK_CHECK(
    vkCreateCommandPool(
      _device,
      &commandPoolInfo,
      nullptr,
      &_im_command_pool
    )
  );

  // Allocate the command buffer for immediate submits
  VkCommandBufferAllocateInfo imCmdAllocInfo {
    .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
    .pNext = nullptr,
    .commandPool = _im_command_pool,
    .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
    .commandBufferCount = 1
  };
  VK_CHECK(
    vkAllocateCommandBuffers(
      _device,
      &imCmdAllocInfo,
      &_im_command_buffer
    )
  );

  // Push it to the deletion queue
  _main_deletion_queue.push_function([=]() {
    vkDestroyCommandPool(_device, _im_command_pool, nullptr);
  });

}

void VulkanEngine::init_sync_structures() {
  // Fence is for GPU to CPU syncronization.
  VkFenceCreateInfo fenceCreateInfo {
    .sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
    .pNext = nullptr,
    .flags = VK_FENCE_CREATE_SIGNALED_BIT
  };

  // Semaphore is for GPU to GPU syncronization.
  VkSemaphoreCreateInfo semaphoreCreateInfo {
    .sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
    .pNext = nullptr,
  };

  for (int i=0; i<FRAME_OVERLAP; ++i) {
    VK_CHECK(vkCreateFence(_device, &fenceCreateInfo, nullptr, &_frames[i]._render_fence));
  
    VK_CHECK(vkCreateSemaphore(_device, &semaphoreCreateInfo, nullptr, &_frames[i]._swapchain_semaphore));
    VK_CHECK(vkCreateSemaphore(_device, &semaphoreCreateInfo, nullptr, &_frames[i]._render_semaphore));
  }

  /* IMMEDIATE SUBMIT SYNC SETUP */
  VK_CHECK(
    vkCreateFence(
      _device,
      &fenceCreateInfo,
      nullptr,
      &_im_fence
    )
  );
  _main_deletion_queue.push_function([=]() {
    vkDestroyFence(_device, _im_fence, nullptr);
  });
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
      // Check if VK_PRESENT_MODE_MAILBOX_KHR is available,
      // and if yes, use the mail box mode.
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

void VulkanEngine::init_descriptors() {
  // Create a descriptor pool that can hold 10 sets with 1 image each.
  std::vector<DescriptorAllocator::PoolSizeRatio> sizes = {
    {
      VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, // Descriptor type to store an image
      1
    }
  };

  // Set up a global descriptor pool
  _global_descriptor_allocator.init_pool(_device, 10, sizes);

  // Make the descriptor set layout for our compute shader
  {
    DescriptorLayoutBuilder builder;
    builder.add_binding(0, VK_DESCRIPTOR_TYPE_STORAGE_IMAGE);
    _draw_image_descriptor_layout = builder.build(
      _device, 
      VK_SHADER_STAGE_COMPUTE_BIT
    );
  }

  // Allocate a descriptor set for our draw image
  _draw_image_descriptors = _global_descriptor_allocator.allocate(_device, _draw_image_descriptor_layout);

  // Get the image view from the canvas
  VkDescriptorImageInfo imageInfo{
    .imageView = _canvas._image_view,
    .imageLayout = VK_IMAGE_LAYOUT_GENERAL
  };

  // Bind the image to the image descriptor
  VkWriteDescriptorSet imageWrite {
    .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
    .pNext = nullptr,

    .dstSet = _draw_image_descriptors,
    .dstBinding = 0,
    .descriptorCount = 1,
    .descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE,
    .pImageInfo = &imageInfo
  };
  vkUpdateDescriptorSets(_device, 1, &imageWrite, 0, nullptr);

  // Make sure both the descriptor allocator and the new layout get cleaned up properly
	_main_deletion_queue.push_function([&]() {
		_global_descriptor_allocator.destroy_pool(_device);

		vkDestroyDescriptorSetLayout(_device, _draw_image_descriptor_layout, nullptr);
	});
}

void VulkanEngine::init_pipelines() {
  init_background_pipelines();
}

void VulkanEngine::init_background_pipelines() {
  // Create the pipeline layout
  VkPipelineLayoutCreateInfo computePipelineLayout {
    .sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
    .pNext = nullptr,
    .setLayoutCount = 1,
    .pSetLayouts = &_draw_image_descriptor_layout
  };

  VK_CHECK(
    vkCreatePipelineLayout(
      _device,
      &computePipelineLayout,
      nullptr,
      &_gradient_pipeline_layout
    )
  );
  
  // Create a shader module
  VkShaderModule computeDrawShader;
  // TODO: How to know the absolute path of the project?
  if (!vkutil::load_shader_module("../../../shaders/gradient.comp.spv", _device, &computeDrawShader)) {
    fmt::println("Error when building the compute shader!");
  }

  // Set a shader stage
  VkPipelineShaderStageCreateInfo stageInfo {
    .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
    .pNext = nullptr,
    .stage = VK_SHADER_STAGE_COMPUTE_BIT,
    .module = computeDrawShader,
    .pName = "main"
  };

  // Compute pipeline info
  VkComputePipelineCreateInfo computePipelineCreateInfo{
    .sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO,
    .pNext = nullptr,
    .stage = stageInfo,
    .layout = _gradient_pipeline_layout,
  };

  // Create the pipeline
  VK_CHECK(
    vkCreateComputePipelines(
      _device,
      VK_NULL_HANDLE,             // Pipeline cache
      1,                          // The number of pipeline to be created
      &computePipelineCreateInfo, // The array of compute pipeline create infos
      nullptr,
      &_gradient_pipeline         // The array of pipeline handles
    )
  );

  // The shader module is not needed anymore,
  // so it can be safely destroyed
  vkDestroyShaderModule(_device, computeDrawShader, nullptr);

  // Push the cleanup stage
  _main_deletion_queue.push_function([&]() {
    vkDestroyPipelineLayout(_device, _gradient_pipeline_layout, nullptr),
    vkDestroyPipeline(_device, _gradient_pipeline, nullptr);
  });
}

void VulkanEngine::immediate_submit(
  std::function<void(VkCommandBuffer cmdBuf)>&& function
) {
  VK_CHECK(vkResetFences(_device, 1, &_im_fence));
  VK_CHECK(vkResetCommandBuffer(_im_command_buffer, 0));

  VkCommandBuffer cmdBuf = _im_command_buffer;

  VkCommandBufferBeginInfo cmdBufBeginInfo {
    .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
    .pNext = nullptr,

    // This command buffer is for one time use
    .flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT,
    // No inheritance command buffer
    .pInheritanceInfo = nullptr,
  };
  VK_CHECK(vkBeginCommandBuffer(cmdBuf, &cmdBufBeginInfo));

  function(cmdBuf);

  VK_CHECK(vkEndCommandBuffer(cmdBuf));

  VkCommandBufferSubmitInfo cmdBufSubmitInfo {
    .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_SUBMIT_INFO,
    .pNext = nullptr,
    .commandBuffer = cmdBuf,
    .deviceMask = 0   // Used when multi-devices need to execute the command
  };

  // Wrap semaphores and command buffer
  VkSubmitInfo2 submitInfo2 {
    .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO_2,
    .pNext = nullptr,

    // The semaphore to be waited before the execution of the commands
    .waitSemaphoreInfoCount = 0,
    .pWaitSemaphoreInfos = nullptr,

    // Command buffer to be submitted
    .commandBufferInfoCount = 1,
    .pCommandBufferInfos = &cmdBufSubmitInfo,

    // The semaphore to be signaled after the execution of the commands
    .signalSemaphoreInfoCount = 0,
    .pSignalSemaphoreInfos = nullptr,
  };

  // Submit the command buffer to the queue and execute it
  // _render_fence will now block until the graphic commands finish execution
  VK_CHECK(vkQueueSubmit2(_graphics_q._handle, 1, &submitInfo2, _im_fence));
  VK_CHECK(vkWaitForFences(_device, 1, &_im_fence, true, 9999999999));
}

void VulkanEngine::init_imgui()
{
	// 1: create descriptor pool for IMGUI
	//  the size of the pool is very oversize, but it's copied from imgui demo
	//  itself.
	VkDescriptorPoolSize pool_sizes[] = { { VK_DESCRIPTOR_TYPE_SAMPLER, 1000 },
		{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000 },
		{ VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000 },
		{ VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000 },
		{ VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000 },
		{ VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000 },
		{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000 },
		{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000 },
		{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000 },
		{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000 },
		{ VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000 } };

	VkDescriptorPoolCreateInfo pool_info = {};
	pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	pool_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
	pool_info.maxSets = 1000;
	pool_info.poolSizeCount = (uint32_t)std::size(pool_sizes);
	pool_info.pPoolSizes = pool_sizes;

	VkDescriptorPool imguiPool;
	VK_CHECK(vkCreateDescriptorPool(_device, &pool_info, nullptr, &imguiPool));

	// 2: initialize imgui library

	// this initializes the core structures of imgui
	ImGui::CreateContext();

	// this initializes imgui for SDL
	ImGui_ImplSDL2_InitForVulkan(_window);

	// this initializes imgui for Vulkan
	ImGui_ImplVulkan_InitInfo init_info = {};
	init_info.Instance = _instance;
	init_info.PhysicalDevice = _selectedGPU;
	init_info.Device = _device;
	init_info.Queue = _graphics_q._handle;
	init_info.DescriptorPool = imguiPool;
	init_info.MinImageCount = 3;
	init_info.ImageCount = 3;
	init_info.UseDynamicRendering = true;

	// dynamic rendering parameters for imgui to use
	init_info.PipelineRenderingCreateInfo = {.sType = VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO};
	init_info.PipelineRenderingCreateInfo.colorAttachmentCount = 1;
	init_info.PipelineRenderingCreateInfo.pColorAttachmentFormats = &_swapchain_image_format;
	

	init_info.MSAASamples = VK_SAMPLE_COUNT_1_BIT;

	ImGui_ImplVulkan_Init(&init_info);

	ImGui_ImplVulkan_CreateFontsTexture();

	// add the destroy the imgui created structures
	_main_deletion_queue.push_function([=]() {
		ImGui_ImplVulkan_Shutdown();
		vkDestroyDescriptorPool(_device, imguiPool, nullptr);
	});
}

void VulkanEngine::draw_imgui(VkCommandBuffer cmdBuf, VkImageView targetImageView) {
  VkRenderingAttachmentInfo colorAttachment = vkst::attachment_info(targetImageView, nullptr, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);

  VkRenderingInfo renderInfo = vkst::rendering_info(_swapchain_extent, &colorAttachment, nullptr);

  vkCmdBeginRendering(cmdBuf, &renderInfo);

  ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), cmdBuf);

  vkCmdEndRendering(cmdBuf);
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
/////////////////////////////////////////////////////
/////////////////////////////////////////////////////
/////////////////////////////////////////////////////
