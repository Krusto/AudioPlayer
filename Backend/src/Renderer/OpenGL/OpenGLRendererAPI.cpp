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
#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/backends/imgui_impl_sdl3.h>
#include <imgui/imgui.h>

#include "SDL3/SDL.h"
#include "SDL3/SDL_opengl.h"
#include <stdexcept>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wall"
#pragma GCC diagnostic ignored "-Wimplicit-int-conversion"
#pragma GCC diagnostic ignored "-Wsign-conversion"
#pragma GCC diagnostic ignored "-Wextra"
#pragma GCC diagnostic ignored "-Wpedantic"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#pragma GCC diagnostic pop

namespace AudioEngine
{

    void OpenGLRendererAPI::CreateWindow()
    {
        if ( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_GAMEPAD ) == 0 )
        {
            LOG_ERROR( "SDL_Init error: %s\n", SDL_GetError() );
            throw std::runtime_error( "Couldn't initialize SDL" );
        }

        SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 3 );
        SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 3 );
        SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );

        m_RendererData->window = new Window;
        m_RendererData->window->name = m_Config.windowName.data();
        m_RendererData->window->data = static_cast<void*>(
                SDL_CreateWindow( m_RendererData->window->name, ( int ) m_Config.initialWidth,
                                  ( int ) m_Config.initialHeight, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE ) );
        if ( m_RendererData->window->data == nullptr ) { throw std::runtime_error( "Couldn't create Window" ); };

        m_RendererData->glContext = SDL_GL_CreateContext( static_cast<SDL_Window*>( m_RendererData->window->data ) );
        if ( m_RendererData->glContext == nullptr ) { throw std::runtime_error( "Couldn't create GL Context" ); };

        if ( !gladLoadGLLoader( reinterpret_cast<GLADloadproc>( SDL_GL_GetProcAddress ) ) )
        {
            throw std::runtime_error( "Couldn't initialize GLAD" );
        }

        SDL_GL_MakeCurrent( ( SDL_Window* ) m_RendererData->window->data, m_RendererData->glContext );
        SDL_GL_SetSwapInterval( 1 );// Enable vsync
        SDL_SetWindowPosition( ( SDL_Window* ) m_RendererData->window->data, SDL_WINDOWPOS_CENTERED,
                               SDL_WINDOWPOS_CENTERED );


        SDL_ShowWindow( ( SDL_Window* ) m_RendererData->window->data );
    }

    Window* OpenGLRendererAPI::GetWindow() { return m_RendererData->window; }

    void OpenGLRendererAPI::Clear( const Color4& color )
    {
        glClear( GL_COLOR_BUFFER_BIT );
        glClearColor( color.r, color.g, color.b, color.a );
    }

    void OpenGLRendererAPI::Present() { SDL_GL_SwapWindow( ( SDL_Window* ) ( m_RendererData->window->data ) ); }

    void OpenGLRendererAPI::BeginFrame()
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();
    }

    void OpenGLRendererAPI::EndFrame()
    {
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData( ImGui::GetDrawData() );
    }

    void OpenGLRendererAPI::Init( const RendererAPIConfig& config )
    {
        m_Config = config;
        m_RendererData = std::make_unique<RendererDataType>();

        CreateWindow();
        InitImGui();
    }

    void OpenGLRendererAPI::Destroy() { SDL_DestroyWindow( static_cast<SDL_Window*>( m_RendererData->window->data ) ); }

    void OpenGLRendererAPI::InitImGui()
    {
        LOG_INFO( "Initializing ImGui..." );
        // Setup Dear ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        ( void ) io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;// Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad; // Enable Gamepad Controls
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;    // IF using Docking Branch

        // Setup Dear ImGui style
        ImGui::StyleColorsDark();
        //ImGui::StyleColorsLight();

        // Setup Platform/Renderer backends
        ImGui_ImplSDL3_InitForOpenGL( ( SDL_Window* ) m_RendererData->window->data, m_RendererData->glContext );
        ImGui_ImplOpenGL3_Init( "#version 130" );
        LOG_INFO( "ImGui initialized" );
    }

    void OpenGLRendererAPI::DestroyImGui()
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplSDL3_Shutdown();
        ImGui::DestroyContext();
    }

    void OpenGLRendererAPI::ResizeViewport( uint32_t width, uint32_t height )
    {
        glViewport( 0, 0, static_cast<GLsizei>( width ), static_cast<GLsizei>( height ) );
    }

    uint32_t OpenGLRendererAPI::GetWindowWidth()
    {
        int width = 0, height = 0;
        SDL_GetWindowSizeInPixels( ( SDL_Window* ) m_RendererData->window->data, &width, &height );
        return static_cast<uint32_t>( width );
    }

    uint32_t OpenGLRendererAPI::GetWindowHeight()
    {
        int width = 0, height = 0;
        SDL_GetWindowSizeInPixels( ( SDL_Window* ) m_RendererData->window->data, &width, &height );
        return static_cast<uint32_t>( height );
    }

    void OpenGLRendererAPI::LoadTexture( std::filesystem::path path, size_t& id, size_t& width, size_t& height,
                                         size_t& channels )
    {
        unsigned char* data =
                stbi_load( path.c_str(), ( int* ) &width, ( int* ) &height, ( int* ) &channels, 4 );// force RGBA
        if ( !data )
        {
            printf( "Failed to load image\n" );
            return;
        }

        // Upload to OpenGL texture (same as before)
        GLuint textureID;
        glGenTextures( 1, &textureID );
        glBindTexture( GL_TEXTURE_2D, textureID );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );

        glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, ( int ) width, ( int ) height, 0, GL_RGBA, GL_UNSIGNED_BYTE,
                      ( void* ) data );

        glBindTexture( GL_TEXTURE_2D, 0 );

        id = textureID;
        stbi_image_free( data );
    }
}// namespace AudioEngine