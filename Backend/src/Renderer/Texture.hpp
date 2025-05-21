/**
 * @file
 * @author Krusto Stoyanov ( k.stoianov2@gmail.com )
 * @brief 
 * @version 1.0
 * @date 
 * 
 * @section LICENSE
 * MIT License
 * 
 * Copyright (c) 2025 Krusto
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 * 
 * @section DESCRIPTION
 * 
 * Texture declarations
 */

#pragma once

#include <cstdint>
#include <filesystem>

namespace AudioEngine
{
    class Texture
    {
    public:
        Texture() = default;
        ~Texture() = default;

    public:
        static Texture Create( std::filesystem::path path );

    public:
        void Load( std::filesystem::path path );

        size_t GetId() const { return m_id; }

        size_t GetWidth() const { return m_width; }

        size_t GetHeight() const { return m_height; }

        size_t GetChannels() const { return m_channels; }

    private:
        Texture( size_t id, size_t width, size_t height, size_t channels )
            : m_id( id ), m_width( width ), m_height( height ), m_channels( channels )
        {}

    private:
        size_t m_id = 0;
        size_t m_width = 0;
        size_t m_height = 0;
        size_t m_channels = 0;
    };

}// namespace AudioEngine