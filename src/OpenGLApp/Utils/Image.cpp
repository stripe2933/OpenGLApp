//
// Created by gomkyung2 on 2023/09/13.
//

#include "OpenGLApp/Utils/Image.hpp"

#include <stdexcept>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

OpenGL::Utils::Image::Image(int width, int height, int channels, const unsigned char *data) noexcept : width { width }, height { height }, channels { channels }, data { data } {

}

OpenGL::Utils::Image OpenGL::Utils::Image::from(const char *filename) {
    int width, height, channels;
    unsigned char *data = stbi_load(filename, &width, &height, &channels, 0);
    if (!data){
        throw std::runtime_error { "Failed to load image." };
    }

    return { width, height, channels, data };
}

OpenGL::Utils::Image::Image(const char *filename) : Image { from(filename) } {

}

OpenGL::Utils::Image::Image(Image &&source) noexcept : width { source.width }, height { source.height }, channels { source.channels }, data { source.data } {
    source.data = nullptr;
}

OpenGL::Utils::Image::~Image() noexcept {
    if (data){
        stbi_image_free(const_cast<unsigned char *>(data));
    }
}