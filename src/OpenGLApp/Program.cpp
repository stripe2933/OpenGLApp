//
// Created by gomkyung2 on 2023/08/29.
//

#include "OpenGLApp/Program.hpp"

#include <fstream>

namespace{
    GLuint createProgram(const OpenGL::Shader &vertex_shader, const OpenGL::Shader &fragment_shader) {
        const GLuint handle = glCreateProgram();
        glAttachShader(handle, vertex_shader.handle);
        glAttachShader(handle, fragment_shader.handle);

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

        return handle;
    }
};

OpenGL::Program::Program(const std::filesystem::path &vertex_shader_path, const std::filesystem::path &fragment_shader_path)
        : handle { createProgram(OpenGL::Shader::fromFile(GL_VERTEX_SHADER, vertex_shader_path),
                                 OpenGL::Shader::fromFile(GL_FRAGMENT_SHADER, fragment_shader_path)) }
{

}

OpenGL::Program::Program(const Shader &vertex_shader, const Shader &fragment_shader)
        : handle { createProgram(vertex_shader, fragment_shader) }
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

void OpenGL::Program::setUniformBlockBinding(const char *name, GLuint binding_point) const {
    const GLuint index = glGetUniformBlockIndex(handle, name);
    glUniformBlockBinding(handle, index, binding_point);
}

void OpenGL::Program::use() const {
    State::setProgram(handle);
}