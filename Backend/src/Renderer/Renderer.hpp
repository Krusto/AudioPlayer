/**
 * @file
 * @author Krusto Stoyanov ( k.stoianov2@gmail.com ) 
 * @coauthor Neyko Naydenov (neyko641@gmail.com)
 * @brief 
 * @version 1.0
 * @date 
 * 
 * @section LICENSE
 * MIT License
 * 
 * Copyright (c) 2025 Krusto, Neyko
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 * 
 * @section DESCRIPTION
 * 
 * Renderer declarations
 */

#pragma once

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "Core/STDTypes.h"
#include "Math/Color.h"
#include "Math/Rect.h"
#include "RendererCommands.hpp"
#include "Texture.hpp"
#include "Window.hpp"

#include <SDL3/SDL_render.h>

#include <string_view>

namespace AudioEngine
{

    class Renderer
    {
    public:
        static RendererResultType Init( std::string_view window_name, uint32_t width, uint32_t height );
        static void Destroy();
        static void Present();

        static void Clear( const Color4& color );

        static void BeginRenderPass();
        static void EndRenderPass();
        static void Flush();

        static void ResizeViewport( uint32_t width, uint32_t height );

        static void LoadTexture( std::filesystem::path path, size_t& id, size_t& width, size_t& height,
                                 size_t& channels );

        static Window* GetWindow();
        static uint32_t GetWindowWidth();
        static uint32_t GetWindowHeight();
    };
}// namespace AudioEngine