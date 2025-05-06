#include "AudioPlayerLayer.hpp"
#include "AudioEngine.hpp"

#include <Core/Log.h>
#include <filesystem>

void AudioPlayerLayer::Init()
{
    m_Window = AudioEngine::Renderer::GetWindow();

    auto workingDirectory = std::filesystem::current_path();
    auto assetsDirectory = workingDirectory / "Assets";
    auto shadersDirectory = assetsDirectory / "Shaders";
    auto audioDirectory = assetsDirectory / "Audio";

    std::string testAudio = audioDirectory / "AudioTest.wav";
    AudioEngine::AudioManager::LoadAudio( "AudioTest", testAudio );
}

void AudioPlayerLayer::OnUpdate( float dt )
{
    ( void ) dt;
    using namespace AudioEngine;

    Renderer::BeginRenderPass();

    Renderer::Clear( Color4{ 0.1f, 0.2f, 0.3f, 1.0f } );

    size_t length = ( size_t ) snprintf( NULL, 0, "%u, %u", x, y );
    text[ length ] = '\0';
    snprintf( text, length + 1, "%d, %d", x, y );

    std::string_view t = std::string_view( text, length );

    Renderer::EndRenderPass();

    //Play audio test
    if ( !AudioEngine::AudioManager::IsAudioPlaying( "AudioTest" ) )
    {
        AudioEngine::AudioManager::PlayAudio( "AudioTest" );
    }

    //Flush command buffer for drawing
    Renderer::Flush();
}

void AudioPlayerLayer::OnMouseMoveEvent( uint32_t width, uint32_t height )
{
    this->x = width;
    this->y = height;
}
