#include "AudioPlayerLayer.hpp"
#include "AudioEngine.hpp"

#include <Core/Log.h>
#include <filesystem>

#include <imgui_impl_opengl3.h>
#include <imgui_impl_sdl3.h>

void AudioPlayerLayer::Init()
{
    m_Window = AudioEngine::Renderer::GetWindow();

    auto workingDirectory = std::filesystem::current_path();
    auto assetsDirectory = workingDirectory / "Assets";
    auto shadersDirectory = assetsDirectory / "Shaders";
    auto audioDirectory = assetsDirectory / "Audio";

    std::string testAudio = audioDirectory / "AudioTest.wav";
    AudioEngine::AudioManager::LoadAudio( "AudioTest", testAudio );

    m_Width = AudioEngine::Renderer::GetWindowWidth();
    m_Height = AudioEngine::Renderer::GetWindowHeight();
}

void AudioPlayerLayer::OnUpdate( float dt )
{
    ( void ) dt;
    using namespace AudioEngine;

    Renderer::Clear( Color4{ 0.1f, 0.2f, 0.3f, 1.0f } );


    Renderer::Flush();
}

void AudioPlayerLayer::OnMouseMoveEvent( uint32_t width, uint32_t height )
{
    this->x = width;
    this->y = height;
}

void AudioPlayerLayer::OnWindowResizeEvent( uint32_t width, uint32_t height )
{
    AudioEngine::Renderer::ResizeViewport( width, height );
    m_Width = width;
    m_Height = height;
}

void AudioPlayerLayer::OnImGuiDraw()
{
    ImGui::SetNextWindowPos( ImVec2( 0, 0 ) );
    ImGui::SetNextWindowSize( ImVec2( static_cast<float>( m_Width ), static_cast<float>( m_Height ) ) );
    ImGui::Begin( "Audio Player", nullptr,
                  ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
                          ImGuiWindowFlags_NoMove );

    ImGui::Text( "Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate,
                 ImGui::GetIO().Framerate );

    ImGui::Text( "Currently playing: %s", AudioEngine::AudioManager::GetCurrentAudioName().data() );

    ImGui::SliderFloat( "Volume", &m_Volume, 0.0f, 1.0f );

    m_Volume = AudioEngine::AudioManager::SetVolume( m_Volume );

    if ( AudioEngine::AudioManager::IsAudioPlaying( "AudioTest" ) )
    {
        if ( ImGui::Button( "Stop Audio Test" ) ) { AudioEngine::AudioManager::StopAudio( "AudioTest" ); }
    }
    else
    {
        if ( ImGui::Button( "Play Audio Test" ) ) { AudioEngine::AudioManager::PlayAudio( "AudioTest" ); }
    }

    ImGui::End();
}