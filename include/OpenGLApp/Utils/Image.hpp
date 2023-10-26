//
// Created by gomkyung2 on 2023/09/13.
//

#pragma once

namespace OpenGL::Utils{
    class Image{
    private:
        Image(int width, int height, int channels, const unsigned char *data) noexcept;

        static Image from(const char *filename);

    public:
        const int width;
        const int height;
        const int channels;
        const unsigned char *data;

        Image(const char *filename);
        Image(Image &&source) noexcept;
        ~Image() noexcept;
    };
}