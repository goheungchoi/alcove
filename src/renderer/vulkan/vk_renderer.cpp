#include "renderer/vulkan/vk_renderer.h"

#include "renderer/vulkan/context/vk_render_device.h"

bool VkRenderer::Init_Win32(void* hInstance, void* hwnd)
{
  _device = new VkRenderDevice;
  _device->Init_Win32(hInstance, hwnd);

  return false;
}

bool VkRenderer::Init_SDL2(void* sdlWindow)
{
  _device = new VkRenderDevice;
  _device->Init_SDL2(sdlWindow);

  return false;
}

bool VkRenderer::Cleanup() 
{
  return false;
}

void VkRenderer::ResizeScreen(unsigned int width, unsigned int height)
{
}

void VkRenderer::BeginDraw()
{
}

void VkRenderer::EndDraw()
{
}

bool VkRenderer::LoadModelFromFile(const char *path)
{
  return false;
}

bool VkRenderer::LoadTextureFromFile(const char *path)
{
  return false;
}
