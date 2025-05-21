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
    auto fontDirectory = assetsDirectory / "Fonts";
    auto iconDirectory = assetsDirectory / "Icons";
    auto openSansFont = fontDirectory / "OpenSans-SemiBold.ttf";

    m_PlayButtonTexture = AudioEngine::Texture::Create( iconDirectory / "play.png" );
    m_PauseButtonTexture = AudioEngine::Texture::Create( iconDirectory / "pause.png" );

    std::string testAudio = audioDirectory / "AudioTest.wav";
    AudioEngine::AudioManager::LoadAudio( "AudioTest", testAudio );

    m_Width = AudioEngine::Renderer::GetWindowWidth();
    m_Height = AudioEngine::Renderer::GetWindowHeight();

    if ( AudioEngine::FileUtils ::Exists( openSansFont ) )
    {
        ImFont* font = ImGui::GetIO().Fonts->AddFontFromFileTTF( openSansFont.c_str(), 18.0f );
        if ( font ) ImGui::GetIO().FontDefault = font;
    }
    else { AudioEngine::LOG_ERROR( "Font file not found: %s\n", openSansFont.c_str() ); }

    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowRounding = 8.0f;   // Rounds window corners (try 5.0f–15.0f)
    style.FrameRounding = 6.0f;    // Rounds buttons, sliders, inputs, etc.
    style.ScrollbarRounding = 6.0f;// Rounds scrollbar grab
    style.GrabRounding = 6.0f;     // Rounds slider/drag handles
}

void AudioPlayerLayer::OnUpdate( float dt )
{
    ( void ) dt;
    using namespace AudioEngine;

    Renderer::Clear( Color4{ 0.0f, 0.0f, 0.0f, 1.0f } );


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

void AudioPlayerLayer::DrawSongList()
{
    if ( ImGui::Button( "Open File" ) ) { m_SelectedFile = AudioEngine::FileUtils::OpenFileDialog(); }

    if ( !m_SelectedFile.empty() )
    {
        auto utf8Str = m_SelectedFile.u8string();
        std::string filename( utf8Str.begin(), utf8Str.end() );
        ImGui::Text( "Selected: %s", filename.c_str() );
    }
}

void AudioPlayerLayer::DrawPlayerControls()
{
    ImGui::PushStyleColor( ImGuiCol_ButtonActive, IM_COL32( 161, 72, 80, 255 ) );
    ImVec2 avail = ImGui::GetContentRegionAvail();// space available inside child
    ImVec2 size( 25, 25 );
    ImVec2 padding = ImGui::GetStyle().FramePadding;// button frame padding

    // Calculate button width including padding (button width = text width + padding * 2)
    float button_width = size.x + padding.x * 2.0f;
    float button_height = size.y + padding.y * 2.0f;

    // Calculate offset to center the button
    float offset_x = ( avail.x - button_width ) * 0.5f;
    if ( offset_x < 0.0f ) offset_x = 0.0f;// prevent negative offset

    // Calculate offset to center the button
    float offset_y = ( avail.y - button_height ) * 0.5f;
    if ( offset_y < 0.0f ) offset_y = 0.0f;// prevent negative offset

    // Set cursor position
    ImGui::SetCursorPosX( ImGui::GetCursorPosX() + offset_x );
    ImGui::SetCursorPosY( ImGui::GetCursorPosY() + offset_y );

    ImDrawList* draw_list = ImGui::GetWindowDrawList();

    ImVec2 pos = ImGui::GetCursorScreenPos();
    float diameter = 64.0f;
    ImVec2 center = ImVec2( pos.x - button_width / 2 + diameter * 0.5f, pos.y - button_height / 2 + diameter * 0.5f );
    // Draw circle behind the button
    draw_list->AddCircleFilled( center, diameter * 0.5f, IM_COL32( 161, 72, 80, 255 ) );// your red-ish color

    if ( AudioEngine::AudioManager::IsAudioPlaying( "AudioTest" ) )
    {
        if ( ImGui::ImageButton( "pause_", ( ImTextureID ) m_PauseButtonTexture.GetId(), size ) )
        {
            AudioEngine::AudioManager::StopAudio( "AudioTest" );
        }
    }
    else
    {
        if ( ImGui::ImageButton( "play_", ( ImTextureID ) m_PlayButtonTexture.GetId(), size ) )
        {
            AudioEngine::AudioManager::PlayAudio( "AudioTest" );
        }
    }

    if ( ImGui::IsItemHovered() )
    {
        draw_list->AddCircleFilled( center, diameter * 0.5f, IM_COL32( 200, 100, 110, 100 ) );// your red-ish color
    }

    ImGui::PopStyleColor();
}

void AudioPlayerLayer::DrawCurrentlyPlaying()
{

    ImGui::PushStyleColor( ImGuiCol_Text, IM_COL32( 160, 150, 140, 255 ) );
    ImGui::PushStyleVar( ImGuiStyleVar_ItemSpacing, ImVec2( 10, 10 ) );

    auto currently_playing = "Currently playing: " + std::string( AudioEngine::AudioManager::GetCurrentAudioName() );
    auto width = ImGui::CalcTextSize( currently_playing.c_str() ).x;

    ImGui::SetCursorPosX( ( ImGui::GetContentRegionAvail().x - width ) * 0.5f );
    ImGui::Text( "%s", currently_playing.c_str() );

    ImGui::SetNextItemWidth( ImGui::GetContentRegionAvail().x * 0.8f );
    ImGui::SetCursorPosX( ( ImGui::GetContentRegionAvail().x * 0.1f ) );
    ImGui::PushID( "Volume" );
    ImGui::SliderFloat( "", &m_Volume, 0.0f, 1.0f );
    ImGui::PopID();
    m_Volume = AudioEngine::AudioManager::SetVolume( m_Volume );

    ImGui::PopStyleVar();
    ImGui::PopStyleColor();
}

void AudioPlayerLayer::DrawLeftPanel()
{
    ImGuiStyle& style = ImGui::GetStyle();
    style.ItemSpacing.x = 0.0f;// no horizontal spacing between items
    style.ItemSpacing.y = 0.0f;// no vertical spacing between items
    style.WindowPadding.x = 0.0f;
    style.WindowPadding.y = 0.0f;
    float total_width = ImGui::GetContentRegionAvail().x;
    float total_height = ImGui::GetContentRegionAvail().y;
    float row1_height = total_height * 0.66f;// 66%

    // top child — push color BEFORE BeginChild
    ImGui::PushStyleColor( ImGuiCol_ChildBg, IM_COL32( 255, 255, 255, 255 ) );
    ImGui::BeginChild( "top", ImVec2( total_width, row1_height ), true, ImGuiWindowFlags_NoMove );
    DrawCurrentlyPlaying();
    ImGui::EndChild();
    ImGui::PopStyleColor();

    // Right child — push color BEFORE BeginChild
    ImGui::PushStyleColor( ImGuiCol_ChildBg, IM_COL32( 204, 96, 103, 255 ) );
    ImGui::BeginChild( "bottom", ImVec2( total_width, total_height - row1_height ), true, ImGuiWindowFlags_NoMove );
    DrawPlayerControls();
    ImGui::EndChild();
    ImGui::PopStyleColor();
}

void AudioPlayerLayer::OnImGuiDraw()
{
    ImGui::PushStyleColor( ImGuiCol_Border, IM_COL32( 0, 0, 0, 0 ) );// transparent border
    ImGui::PushStyleColor( ImGuiCol_Button, IM_COL32( 163, 77, 81, 255 ) );
    ImGui::PushStyleColor( ImGuiCol_ButtonHovered, IM_COL32( 200, 100, 110, 0 ) );
    ImGui::PushStyleColor( ImGuiCol_ButtonActive, IM_COL32( 140, 60, 70, 255 ) );
    ImGui::PushStyleColor( ImGuiCol_SliderGrab, IM_COL32( 163, 77, 81, 255 ) );
    ImGui::PushStyleColor( ImGuiCol_SliderGrabActive, IM_COL32( 200, 100, 110, 255 ) );
    ImGui::PushStyleColor( ImGuiCol_FrameBg, IM_COL32( 120, 40, 45, 255 ) );
    ImGui::PushStyleColor( ImGuiCol_FrameBgHovered, IM_COL32( 150, 50, 60, 255 ) );
    ImGui::PushStyleColor( ImGuiCol_FrameBgActive, IM_COL32( 163, 77, 81, 255 ) );
    ImGui::PushStyleColor( ImGuiCol_WindowBg, IM_COL32( 0, 0, 0, 0 ) );

    ImGui::SetNextWindowPos( ImVec2( 0, 0 ) );
    ImGui::SetNextWindowSize( ImVec2( static_cast<float>( m_Width ), static_cast<float>( m_Height ) ) );

    ImGuiStyle& style = ImGui::GetStyle();
    style.ItemSpacing.x = 0.0f;// no horizontal spacing between items
    style.WindowPadding.x = 0.0f;
    style.WindowPadding.y = 0.0f;

    ImGui::Begin( "Audio Player", nullptr,
                  ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
                          ImGuiWindowFlags_NoMove );

    float total_width = ImGui::GetContentRegionAvail().x;
    float total_height = ImGui::GetContentRegionAvail().y;
    float col1_width = total_width * 0.66f;// 66%

    // Left child — push color BEFORE BeginChild
    ImGui::PushStyleColor( ImGuiCol_ChildBg, IM_COL32( 255, 255, 255, 255 ) );
    ImGui::BeginChild( "left", ImVec2( col1_width, total_height ), false, ImGuiWindowFlags_NoMove );

    DrawLeftPanel();

    ImGui::EndChild();
    ImGui::PopStyleColor();
    ImGui::SameLine();

    // Right child — push color BEFORE BeginChild
    ImGui::PushStyleColor( ImGuiCol_ChildBg, IM_COL32( 163, 77, 82, 255 ) );
    ImGui::BeginChild( "right", ImVec2( total_width - col1_width, total_height ), false, ImGuiWindowFlags_NoMove );
    DrawSongList();
    ImGui::EndChild();
    ImGui::PopStyleColor();

    ImGui::End();

    ImGui::PopStyleColor( 10 );// pop all your earlier pushes
}