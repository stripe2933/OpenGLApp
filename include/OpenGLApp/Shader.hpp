//
// Created by gomkyung2 on 2023/09/10.
//

#pragma once

#include <filesystem>
#include <GL/glew.h>

namespace OpenGL{
    class Shader{
    private:
        Shader(GLuint handle);
        Shader(const Shader&) = delete; // Shader cannot be copied.

    public:
        const GLuint handle;

        ~Shader() noexcept;

        static Shader fromSource(GLenum type, const char *source);
        static Shader fromFile(GLenum type, const std::filesystem::path &filename);
    };
}