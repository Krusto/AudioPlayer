#include "Application.hpp"
#include "AudioEngine.hpp"
#include <Core/Log.h>
#include <glad/glad.h>

namespace AudioEngine
{
    // inline static Application s_Instance;

    ApplicationResult Application::Create( std::string_view name, uint32_t width, uint32_t height )
    {
        CLogCreate();

        CLogAttachTerminalHandler( LogPolicy::OVERWRITE_POLICY );

        auto renderer_result = Renderer::Init( name, width, height );
        if ( renderer_result != AudioEngine::RendererResultType::Renderer_Result_Success )
        {
            return ApplicationResult_Fail;
        }

        LOG_INFO( "Initializing AudioManager..." );
        auto audio_manager_result = AudioManager::Init();
        if ( audio_manager_result != AudioManager_Result_Success )
        {
            throw std::runtime_error( "Couldn't initialize AudioManager" );
        }

        return ApplicationResult_Success;
    }

    void Application::Run()
    {
        LayerStack::InitLayers();
        Event event;
        while ( true )
        {
            if ( PollEvents( &event ) )
            {
                if ( event.type == EVENT_QUIT ) { break; }

                LayerStack::OnEvent( &event );
            }

            LayerStack::Begin();
            LayerStack::OnUpdate( 1.0f );
            LayerStack::OnImGuiDraw();
            LayerStack::End();

            Renderer::Present();
        }
    }

    void Application::Close()
    {
        LayerStack::DestroyLayers();
        AudioManager::Destroy();
        Renderer::Destroy();
    }

}// namespace AudioEngine