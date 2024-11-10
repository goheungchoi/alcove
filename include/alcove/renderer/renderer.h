#pragma once

class IRenderer {
public:
  virtual bool Init_Win32(void* hInstance, void* hwnd) = 0;
  virtual bool Init_SDL2(void* sdlWindow) = 0;

  virtual bool Cleanup() = 0;

  virtual void ResizeScreen(unsigned int width, unsigned int height) = 0;

  virtual void BeginFrame() = 0;

  virtual void BeginDraw() = 0;

  virtual void EndDraw() = 0;

  virtual void EndFrame() = 0;

  virtual void BindPipeline() = 0;

  virtual void BindResource() = 0;



  virtual bool CreateMesh(const char* path) = 0;

  virtual bool CreateTexture(const char* path) = 0;

};
