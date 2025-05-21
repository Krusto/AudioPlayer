#include "../FileUtils.hpp"

#ifdef __linux__

#include <array>
#include <cwchar>
#include <locale>
#include <sys/statvfs.h>
#include <vector>

namespace AudioEngine
{
    bool FileUtils::Exists( const std::filesystem::path& path ) { return std::filesystem::exists( path ); }

    size_t FileUtils::GetMaxFilePathLength()
    {
        struct statvfs fs_stats;
        if ( statvfs( ".", &fs_stats ) == 0 ) { return fs_stats.f_namemax; }
        else { throw std::runtime_error( "statvfs failed" ); }
    }

    // Helper to read output from a pipe into a UTF-8 string
    inline static std::string ReadUtf8FromPipe( FILE* pipe, size_t bufferSize )
    {
        std::vector<char> buffer( bufferSize );
        std::string result;

        while ( fgets( buffer.data(), static_cast<int>( buffer.size() ), pipe ) != nullptr )
        {
            result += buffer.data();
        }

        if ( !result.empty() && result.back() == '\n' ) { result.pop_back(); }

        return result;
    }

    // Helper to convert UTF-8 string to wide string using system locale
    inline static std::wstring Utf8ToWString( const std::string& utf8Str )
    {
        std::setlocale( LC_ALL, "" );// Use system locale

        std::mbstate_t state{};
        const char* src = utf8Str.c_str();
        size_t len = std::mbsrtowcs( nullptr, &src, 0, &state );
        if ( len == static_cast<size_t>( -1 ) ) { return {}; }

        std::wstring wstr( len, L'\0' );
        src = utf8Str.c_str();// Reset pointer
        std::mbsrtowcs( &wstr[ 0 ], &src, len, &state );
        return wstr;
    }

    std::filesystem::path FileUtils::OpenFileDialog( std::filesystem::path workingDirectory )
    {
        const auto pathLength = FileUtils::GetMaxFilePathLength();

        std::string command = "zenity --file-selection";

        if ( !workingDirectory.empty() )
        {
            workingDirectory = std::filesystem::absolute( workingDirectory );
            std::string dirStr = workingDirectory.string();

            if ( !dirStr.empty() && dirStr.back() != '/' ) { dirStr += '/'; }

            command += " --filename=\"" + dirStr + "\"";
        }

        std::unique_ptr<FILE, decltype( &pclose )> pipe( popen( command.c_str(), "r" ), pclose );

        if ( !pipe ) { return {}; }

        std::string utf8Path = ReadUtf8FromPipe( pipe.get(), pathLength );
        std::wstring widePath = Utf8ToWString( utf8Path );

        return std::filesystem::path( widePath );
    }

}// namespace AudioEngine

#endif