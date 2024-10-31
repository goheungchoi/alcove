#pragma once

#include "renderer/renderer.h"

class VkRenderDevice;

class VkRenderer final : public IRenderer {
  VkRenderDevice* _device{nullptr};

public:
  bool Init_Win32(void* hInstance, void* hwnd) override;
  bool Init_SDL2(void* sdlWindow) override;

  bool Cleanup() override;

  void ResizeScreen(unsigned int width, unsigned int height) override;

  void BeginDraw() override;

  void EndDraw() override;

  bool LoadModelFromFile(const char* path) override;

  bool LoadTextureFromFile(const char* path) override;
};

