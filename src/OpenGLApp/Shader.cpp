//
// Created by gomkyung2 on 2023/09/10.
//

#include "OpenGLApp/Shader.hpp"

#include <fstream>
#include <cassert>

namespace {
    /**
     * @brief Read the contents of a file into a string.
     *
     * @param filename The path to the file.
     * @return std::string The contents of the file.
     * @note In debug mode, file open assertion is performed.
     */
    std::string readFile(const std::filesystem::path &filename) {
        std::ifstream file { filename };
        assert(file.is_open());

        return { std::istreambuf_iterator<char> { file }, std::istreambuf_iterator<char> { }};
    }

    GLuint createShader(GLenum type, const char *source) {
        assert(type == GL_VERTEX_SHADER || type == GL_FRAGMENT_SHADER);

        const GLuint handle = glCreateShader(type);
        glShaderSource(handle, 1, &source, nullptr);
        glCompileShader(handle);

#ifndef NDEBUG
        static GLint success;
        glGetShaderiv(handle, GL_COMPILE_STATUS, &success);
        if (!success) {
            static std::array<char, 512> info_log;
            glGetShaderInfoLog(handle, info_log.size(), nullptr, info_log.data());

            throw std::runtime_error { info_log.data() };
        }
#endif

        return handle;
    }
};

OpenGL::Shader::Shader(GLuint handle) : handle { handle } {

}

OpenGL::Shader::~Shader() noexcept {
    glDeleteShader(handle);
}

OpenGL::Shader OpenGL::Shader::fromSource(GLenum type, const char *source) {
    return { ::createShader(type, source) };
}

OpenGL::Shader OpenGL::Shader::fromFile(GLenum type, const std::filesystem::path &filename) {
    return fromSource(type, readFile(filename).c_str());
}
