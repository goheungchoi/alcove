// #define GLM_FORCE_RADIANS
// #define GLM_FORCE_DEPTH_ZERO_TO_ONE
// #include <glm/vec4.hpp>
// #include <glm/mat4x4.hpp>
// #include <vulkan/vulkan.h>

// #define SDL_MAIN_HANDLED
// #include "SDL.h"

// #include <iostream>

#include <core/engine/vk_engine.h>

int main(int argc, char *argv[])
{
    // SDL_SetMainReady();
    // SDL_Init(SDL_INIT_VIDEO);
    // SDL_Quit();

    // glm::vec4 v(0.f, 0.f, 0.f, 0.f);
    // std::cout << v.length << std::endl;
    try {
        VulkanEngine engine;

        engine.init();

        engine.run();

        engine.cleanup();
    } catch (const std::exception& e) {
        // Handle the exception message
        // TODO: Need a logger!
        fmt::print("{}", e.what());
    }

    return 0;
}