#pragma once

#include "renderer/renderer.h"

class VkRenderDevice;

class VkRenderer final : public IRenderer {
  
  // TODO: VkTexture data and handle bind map
  // std::unordered_map<Handle, VkTextureData> _textureHandleBind;
  
  VkRenderDevice* _device{nullptr};

public:
  bool Init_Win32(void* hInstance, void* hwnd) override;
  bool Init_SDL2(void* sdlWindow) override;

  bool Cleanup() override;

  void ResizeScreen(unsigned int width, unsigned int height) override;

  void BeginFrame() override;

  void BeginDraw() override;

  void EndDraw() override;

  void EndFrame() override;

  void BindPipeline() override;

  void BindResource() override;

  bool CreateMesh(const char* path) override;

  bool CreateTexture(const char* path) override;
};

