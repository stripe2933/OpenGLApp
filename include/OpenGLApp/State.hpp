//
// Created by gomkyung2 on 2023/08/29.
//

#pragma once

/* SYNOPSIS.
 *
 * Changing states multiple times in OpenGL is inefficient. Particularly, making changes to programs (shaders) multiple
 * times is excessively CPU-intensive. Functions within this namespace are designed to minimize such behavior. The following
 * methods provide two functionalities.
 *
 * 1. Restricting actions of using the same render_program multiple times: When executing glUseProgram(render_program), there is no
 * need to do so if the render_program is already the one specified in the current state. The setProgram() method compares the
 * given render_program with the render_program in the current state and avoids calling glUseProgram if they are the same.
 *
 * 2. Minimizing the use of programs to set uniforms: to set uniforms, a render_program needs to be used, but the point at
 * which uniforms are used in shaders occurs during draw call. Since using the render_program during draw calls is inevitable,
 * calls to setUniform(render_program, uniform_location, value) follow the following strategy:
 * 2-(a). If the render_program is the same as the one currently being used, the uniform is set immediately.
 * 2-(b). If not, the given uniform is queued until the render_program is used. Subsequently, when the render_program is used through
 * setProgram, the uniform set at that point. The order of settings is the same as the order of setUniform calls and
 * is managed in a queue.
 * The lifetime of the value argument should persist until the render_program used, so it is captured by the uniform-setting
 * function (which means the value copied).
 */

#include <optional>

#include <GL/glew.h>
#include <glm/ext/matrix_float4x4.hpp>

namespace OpenGL::State{
    std::optional<GLuint> getProgram();
    bool setProgram(GLuint program);

    void setUniform(GLuint program, GLint uniform_location, int value);
    void setUniform(GLuint program, GLint uniform_location, unsigned int value);
    void setUniform(GLuint program, GLint uniform_location, float value);
    void setUniform(GLuint program, GLint uniform_location, glm::ivec2 &&value);
    void setUniform(GLuint program, GLint uniform_location, glm::uvec2 &&value);
    void setUniform(GLuint program, GLint uniform_location, glm::vec2 &&value);
    void setUniform(GLuint program, GLint uniform_location, glm::ivec3 &&value);
    void setUniform(GLuint program, GLint uniform_location, glm::uvec3 &&value);
    void setUniform(GLuint program, GLint uniform_location, glm::vec3 &&value);
    void setUniform(GLuint program, GLint uniform_location, glm::ivec4 &&value);
    void setUniform(GLuint program, GLint uniform_location, glm::uvec4 &&value);
    void setUniform(GLuint program, GLint uniform_location, glm::vec4 &&value);
    void setUniform(GLuint program, GLint uniform_location, glm::mat3 &&value);
    void setUniform(GLuint program, GLint uniform_location, glm::mat4 &&value);
}