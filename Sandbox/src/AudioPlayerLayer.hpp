#pragma once

#include <AudioEngine.hpp>

class AudioPlayerLayer: public AudioEngine::Layer
{

public:
    AudioPlayerLayer() = default;
    ~AudioPlayerLayer() = default;

public:
    virtual void Init() override;

    virtual void OnAttach() override {}

    virtual void OnUpdate( float dt ) override;

    virtual void OnDetach() override {}

    virtual void Destroy() override {}

    virtual void OnImGuiDraw() override {}

    virtual void OnMouseButtonEvent( float x, float y, MouseButtonEventType type, MouseButton button ) override
    {
        ( void ) x;
        ( void ) y;
        ( void ) type;
        ( void ) button;
    }

    virtual void OnMouseMoveEvent( uint32_t width, uint32_t height ) override;

    virtual void OnKeyboardEvent( uint32_t key, KeyEventType type ) override
    {
        ( void ) type;
        ( void ) key;
    }

    virtual void OnWindowResizeEvent( uint32_t width, uint32_t height ) override
    {
        ( void ) width;
        ( void ) height;
    }

    virtual void OnWindowShouldCloseEvent() override {}

    virtual void Begin() override {}

    virtual void End() override {}

private:
    AudioEngine::Window* m_Window;
    uint32_t x{}, y{};
    char text[ 100 ];
};