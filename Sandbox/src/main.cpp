#include "AudioPlayerLayer.hpp"
#include <AudioEngine.hpp>

int main()
{

    AudioEngine::Application::Create( "Audio Player", 1280, 720 );
    AudioEngine::LayerStack::PushLayer<AudioPlayerLayer>( "AudioPlayer" );
    AudioEngine::Application::Run();
    AudioEngine::Application::Close();

    return 0;
}