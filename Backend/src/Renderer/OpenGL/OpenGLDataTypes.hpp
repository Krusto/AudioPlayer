#pragma once
#include <Core/STDTypes.h>
#include <Renderer/RendererAPI.hpp>
#include <Renderer/RendererCommands.hpp>
#include <Renderer/Window.hpp>
#include <SDL3/SDL_render.h>
#include <algorithm>// Necessary for std::clamp
#include <array>
#include <cstdint>
#include <fstream>
#include <limits>// Necessary for std::numeric_limits
#include <optional>
#include <set>
#include <string>
#include <vector>

namespace AudioEngine
{
    struct RendererDataType {
        std::vector<RendererCommand*> command_stack;
        AudioEngine::Window* window;
        SDL_Renderer* renderer;
        SDL_Surface* surface;
        SDL_GLContext glContext;

        float width;
        float height;
    };


}// namespace AudioEngine