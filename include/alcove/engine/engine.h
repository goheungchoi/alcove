#pragma once

class IRenderer;

class Engine {

  IRenderer* renderer{ nullptr };

public:

  void Init();

  void Run();

  void Cleanup();

private:

  struct Impl* pImpl{ nullptr };

};
