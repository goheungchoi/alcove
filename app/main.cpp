#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <vulkan/vulkan.h>

#define SDL_MAIN_HANDLED
#include "SDL.h"

#include <iostream>

int main(int argc, char *argv[])
{
    SDL_SetMainReady();
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Quit();

    glm::vec4 v(0.f, 0.f, 0.f, 0.f);
    std::cout << v.length << std::endl;

    return 0;
}