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
#include "Window.hpp"
#include <Math/Color.h>
#include <cstdint>
#include <filesystem>
#include <string_view>

namespace AudioEngine
{

    enum class RendererAPIType
    {
        Vulkan,
        OpenGL,
        None
    };

    struct RendererAPIConfig {
        std::filesystem::path workingDirectory;
        std::filesystem::path assetsDirectory;
        std::filesystem::path shadersDirectory;
        std::string_view windowName;
        uint32_t initialWidth;
        uint32_t initialHeight;
    };

    class RendererAPI

    {
    public:
        inline static RendererAPI* GetInstance() { return s_Instance; }

        static void CreateRendererAPI();
        static void DestroyRendererAPI();

    public:
        virtual ~RendererAPI() = default;

        virtual void Init( const RendererAPIConfig& config ) = 0;
        virtual void Destroy() = 0;
        virtual Window* GetWindow() = 0;
        virtual void Clear( const Color4& color ) = 0;
        virtual void Present() = 0;
        virtual void BeginFrame() = 0;
        virtual void EndFrame() = 0;
        virtual void ResizeViewport( uint32_t width, uint32_t height ) = 0;
        virtual uint32_t GetWindowWidth() = 0;
        virtual uint32_t GetWindowHeight() = 0;
        virtual void LoadTexture( std::filesystem::path path, size_t& id, size_t& width, size_t& height,
                                  size_t& channels ) = 0;

    public:
        inline static RendererAPI* s_Instance;
        inline static constexpr RendererAPIType s_APIType = RendererAPIType::OpenGL;
    };

}// namespace AudioEngine