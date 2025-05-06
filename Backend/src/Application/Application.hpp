#pragma once
#include <Core/STDTypes.h>
#include <string_view>

namespace AudioEngine
{
    typedef enum ApplicationResult
    {
        ApplicationResult_Success = 0,
        ApplicationResult_Fail = 1,
    } ApplicationResult;

    class Application
    {
    public:
        Application() = default;
        ~Application() = default;

    public:
        static ApplicationResult Create( std::string_view name, uint32_t width, uint32_t height );

        static void Run();

        static void Close();
    };
}// namespace AudioEngine