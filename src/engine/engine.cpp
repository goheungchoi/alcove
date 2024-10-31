#include "engine/engine.h"

#include <chrono>
#include <thread>

// SDL
#include <SDL.h>        // The main SDL library data for opening a window and input
#include <SDL_vulkan.h> // Vulkan-specific flags and functionality for opening a Vulkan-compatible window

// Dear ImGui
#include "imgui.h"
#include "../res/bindings/imgui_impl_sdl2.h"
#include "../res/bindings/imgui_impl_vulkan.h"

// {fmt}
#include <fmt/core.h>
#include <fmt/format.h>

//
#include "renderer/vulkan/vk_renderer.h"

// TODO: Need a config file to set the window extent
constexpr uint32_t WINDOW_WIDTH{ 1700U };
constexpr uint32_t WINDOW_HEIGHT{ 900U };

struct Impl {

  SDL_Window* GetSDLWindow() {
    ///// Initialize SDL and create a window with it.
    SDL_Init(SDL_INIT_VIDEO);

    SDL_WindowFlags window_flags = (SDL_WindowFlags) (
      SDL_WINDOW_VULKAN |   // Window for Vulkan
      SDL_WINDOW_RESIZABLE  // Window is resizable (must recreate a swapchain)
    );

    return SDL_CreateWindow(   // Create a window and store on the `_window` member var
      "Alcove",
      SDL_WINDOWPOS_UNDEFINED,
      SDL_WINDOWPOS_UNDEFINED,
      WINDOW_WIDTH,
      WINDOW_HEIGHT,
      window_flags
    );
  }
};


void Engine::Init()
{
  pImpl = new Impl;

  

}

void Engine::Run()
{



}

void Engine::Cleanup()
{

}
