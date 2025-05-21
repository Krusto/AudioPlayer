#pragma once
#include <filesystem>

namespace AudioEngine
{

    class FileUtils
    {
    public:
        FileUtils() = delete;
        ~FileUtils() = delete;

    public:
        static bool Exists( const std::filesystem::path& path );

        static size_t GetMaxFilePathLength();

        static std::filesystem::path
        OpenFileDialog( std::filesystem::path workingDirectory = std::filesystem::current_path() );
    };

}// namespace AudioEngine