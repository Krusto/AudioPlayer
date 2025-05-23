#pragma once

namespace AudioEngine
{
    /***********************************************************************************************************************
    Type definitions
    ***********************************************************************************************************************/
    enum class RendererResultType : int
    {
        Renderer_Result_None = 0,
        Renderer_Result_Success,
        Renderer_Result_Error
    };

    enum class RendererCommandType : int
    {
        RendererCommandType_None = 0,
        RendererCommandType_BindShader,
        RendererCommandType_BindTexture,
        RendererCommandType_Clear,
        RendererCommandType_DrawQuad,
        RendererCommandType_DrawTexture,
        RendererCommandType_DrawCircle,
        RendererCommandType_DrawText
    };

    struct RendererCommandDrawQuad {
        float x;
        float y;
        float width;
        float height;
        float r;
        float g;
        float b;
        float a;
    };

    struct RendererCommandClear {
        float r;
        float g;
        float b;
        float a;
    };

    enum class RendererTextAlignAttribute : int
    {
        TextAlign_TopLeft = 0,
        TextAlign_TopCenter,
        TextAlign_TopRight,
        TextAlign_Left,
        TextAlign_Center,
        TextAlign_Right,
        TextAlign_BottomLeft,
        TextAlign_BottomCenter,
        TextAlign_BottomRight,
    };

    struct RendererCommandDrawText {
        char* text;
        void* font;
        float x;
        float y;
        float r;
        float g;
        float b;
        float a;
        RendererTextAlignAttribute align;
    };

    struct RendererCommandDrawCircle {
        float x;
        float y;
        float radius;
        float r;
        float g;
        float b;
        float a;
    };

    struct RendererCommandNode {

        void* data;
        void* next;
        RendererCommandType type;
    };

    using RendererCommand = void;
}// namespace AudioEngine