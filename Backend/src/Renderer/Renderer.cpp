/**
 * @file
 * @author Krusto Stoyanov ( k.stoianov2@gmail.com ) 
 * @coauthor Neyko Naydenov ( neyko641@gmail.com )
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
 * Renderer definitions
 */


/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "Renderer.hpp"
#include "RendererAPI.hpp"
#include "Texture.hpp"
#include <Core/Log.h>
#include <string_view>

namespace AudioEngine
{

    RendererResultType Renderer::Init( std::string_view window_name, uint32_t width, uint32_t height )
    {
        RendererAPIConfig config;
        config.workingDirectory = std::filesystem::current_path();
        config.assetsDirectory = config.workingDirectory / "Assets";
        config.shadersDirectory = config.assetsDirectory / "Shaders";
        config.windowName = window_name;
        config.initialWidth = width;
        config.initialHeight = height;

        LOG_INFO( "Initializing renderer..." );
        LOG_INFO( "Working directory: %s", config.workingDirectory.c_str() );
        LOG_INFO( "Assets directory: %s", config.assetsDirectory.c_str() );
        LOG_INFO( "Shaders directory: %s", config.shadersDirectory.c_str() );
        LOG_INFO( "Window name: %s", config.windowName.data() );
        LOG_INFO( "Initial width: %d", config.initialWidth );
        LOG_INFO( "Initial height: %d", config.initialHeight );

        RendererAPI::CreateRendererAPI();
        RendererAPI::GetInstance()->Init( config );
        return RendererResultType::Renderer_Result_Success;
    }

    void Renderer::Destroy() { RendererAPI::GetInstance()->Destroy(); }

    void Renderer::Present() { RendererAPI::GetInstance()->Present(); }

    void Renderer::Clear( const Color4& color )
    {
        // RendererCommandClear clear;
        // clear.r = ( uint8_t ) color.r;
        // clear.g = ( uint8_t ) color.g;
        // clear.b = ( uint8_t ) color.b;
        // clear.a = ( uint8_t ) color.a;
        RendererAPI::GetInstance()->Clear( color );
    }

    void Renderer::BeginRenderPass() {}

    void Renderer::EndRenderPass() {}

    void Renderer::Flush() {}

    Window* Renderer::GetWindow() { return RendererAPI::GetInstance()->GetWindow(); }


}// namespace AudioEngine