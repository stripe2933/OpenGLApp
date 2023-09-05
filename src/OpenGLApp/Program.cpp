//
// Created by gomkyung2 on 2023/08/29.
//

#include "OpenGLApp/Program.hpp"

#include <fstream>

namespace{
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

        return { std::istreambuf_iterator<char> { file }, std::istreambuf_iterator<char> {} };
    }

    GLuint createShader(GLenum type, const char *source) {
        assert(type == GL_VERTEX_SHADER || type == GL_FRAGMENT_SHADER);

        const GLuint handle = glCreateShader(type);
        glShaderSource(handle, 1, &source, nullptr);
        glCompileShader(handle);

#ifndef NDEBUG
        static GLint success;
        glGetShaderiv(handle, GL_COMPILE_STATUS, &success);
        if (!success){
            static std::array<char, 512> info_log;
            glGetShaderInfoLog(handle, info_log.size(), nullptr, info_log.data());

            throw std::runtime_error { info_log.data() };
        }
#endif

        return handle;
    }

    GLuint createProgram(const std::filesystem::path &vertex_shader_path, const std::filesystem::path &fragment_shader_path) {
        const auto vertex_shader_source = readFile(vertex_shader_path);
        GLuint vertex_shader = createShader(GL_VERTEX_SHADER, vertex_shader_source.c_str());

        const auto fragment_shader_source = readFile(fragment_shader_path);
        GLuint fragment_shader = createShader(GL_FRAGMENT_SHADER, fragment_shader_source.c_str());

        const GLuint handle = glCreateProgram();
        glAttachShader(handle, vertex_shader);
        glAttachShader(handle, fragment_shader);

        glLinkProgram(handle);

#ifndef NDEBUG
        static GLint success;
        glGetProgramiv(handle, GL_LINK_STATUS, &success);
        if (!success){
            static std::array<char, 512> info_log;
            glGetProgramInfoLog(handle, info_log.size(), nullptr, info_log.data());

            throw std::runtime_error { info_log.data() };
        }
#endif

        glDeleteShader(vertex_shader);
        glDeleteShader(fragment_shader);

        return handle;
    }
};

OpenGL::Program::Program(const std::filesystem::path &vertex_shader_path, const std::filesystem::path &fragment_shader_path)
        : handle { createProgram(vertex_shader_path, fragment_shader_path) }
{

}

OpenGL::Program::~Program() noexcept {
    glDeleteProgram(handle);
}

GLint OpenGL::Program::getUniformLocation(std::string_view name) const {
    auto it = std::ranges::find(uniform_locations, name, [](const auto &pair) { return pair.first; });
    if (it == uniform_locations.cend()){
        std::string name_string { name };
        GLint location = glGetUniformLocation(handle, name_string.c_str());
        return uniform_locations.emplace_back(std::move(name_string), location).second;
    }
    return it->second;
}

void OpenGL::Program::use() const {
    State::setProgram(handle);
}