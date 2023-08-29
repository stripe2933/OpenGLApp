//
// Created by gomkyung2 on 2023/08/29.
//

#pragma once

#include <filesystem>
#include <GL/glew.h>
#include "State.hpp"

namespace OpenGL{
/**
 * @brief A wrapper class for OpenGL program object.
 * @note This class follows RAII structure, so program is created when the object is constructed and deleted when the object is destructed.
 */
    struct Program{
    private:
        mutable std::vector<std::pair<std::string, GLint>> uniform_locations; // TODO: change to flat_map

    public:
        const GLuint handle;

        /**
         * @brief Construct a new Program object.
         * @param vertex_shader_path Path to the vertex shader source file.
         * @param fragment_shader_path Path to the fragment shader source file.
         * @throw In debug mode, each shader's compilation and program's linking state is checked and throw \p std::runtime_error if it fails.
         */
        Program(const std::filesystem::path &vertex_shader_path, const std::filesystem::path &fragment_shader_path);
        ~Program() noexcept;

        /**
         * @brief Get the uniform location of the uniform variable of name \p name .
         * @param name Name of the uniform variable.
         * @return Uniform location in \p GLint
         */
        GLint getUniformLocation(std::string_view name) const;

        /**
         * @brief Set the uniform variable of \p name to \p value .
         * @param name Name of the uniform variable.
         * @param value Value to set.
         */
        void setUniform(std::string_view name, auto value);

        /**
         * @brief Method for calling \p glUseProgram .
         */
        void use() const;
    };
};

void OpenGL::Program::setUniform(std::string_view name, auto value) {
    OpenGL::State::setUniform(handle, getUniformLocation(name), std::move(value));
}