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
#include "OpenGLRendererAPI.hpp"
#include "Core/Log.h"
#include "Renderer/Window.hpp"

#include <glad/glad.h>

#include "SDL3/SDL.h"
#include "SDL3/SDL_opengl.h"
#include <stdexcept>

namespace AudioEngine
{

    void OpenGLRendererAPI::CreateWindow()
    {
        if ( SDL_Init( SDL_INIT_VIDEO ) == 0 )
        {
            LOG_ERROR( "SDL_Init error: %s\n", SDL_GetError() );
            throw std::runtime_error( "Couldn't initialize SDL" );
        }

        SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 3 );
        SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 3 );
        SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );

        m_RendererData->window = new Window;
        m_RendererData->window->name = m_Config.windowName.data();
        m_RendererData->window->data =
                static_cast<void*>( SDL_CreateWindow( m_RendererData->window->name, ( int ) m_Config.initialWidth,
                                                      ( int ) m_Config.initialHeight, SDL_WINDOW_OPENGL ) );
        if ( m_RendererData->window->data == nullptr ) { throw std::runtime_error( "Couldn't create Window" ); };

        SDL_GLContext glContext = SDL_GL_CreateContext( static_cast<SDL_Window*>( m_RendererData->window->data ) );
        if ( glContext == nullptr ) { throw std::runtime_error( "Couldn't create GL Context" ); };

        if ( !gladLoadGLLoader( reinterpret_cast<GLADloadproc>( SDL_GL_GetProcAddress ) ) )
        {
            throw std::runtime_error( "Couldn't initialize GLAD" );
        }
    }

    Window* OpenGLRendererAPI::GetWindow() { return m_RendererData->window; }

    void OpenGLRendererAPI::Clear( const Color4& color )
    {
        glClear( GL_COLOR_BUFFER_BIT );
        glClearColor( color.r, color.g, color.b, color.a );
    }

    void OpenGLRendererAPI::Present() { SDL_GL_SwapWindow( ( SDL_Window* ) ( m_RendererData->window->data ) ); }

    void OpenGLRendererAPI::Init( const RendererAPIConfig& config )
    {
        m_Config = config;
        m_RendererData = std::make_unique<RendererDataType>();

        CreateWindow();
    }

    void OpenGLRendererAPI::Destroy() { SDL_DestroyWindow( static_cast<SDL_Window*>( m_RendererData->window->data ) ); }


}// namespace AudioEngine