#pragma once

class IRenderer {
public:
  virtual bool Init_Win32(void* hInstance, void* hwnd) = 0;
  virtual bool Init_SDL2(void* sdlWindow) = 0;

  virtual bool Cleanup() = 0;

  virtual void ResizeScreen(unsigned int width, unsigned int height) = 0;

  virtual void BeginDraw() = 0;

  virtual void EndDraw() = 0;

  virtual bool LoadModelFromFile(const char* path) = 0;

  virtual bool LoadTextureFromFile(const char* path) = 0;

};
