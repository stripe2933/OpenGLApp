//
// Created by gomkyung2 on 2023/08/29.
//

#include "OpenGLApp/State.hpp"

#include <map>
#include <queue>
#include <functional>

#include <glm/gtc/type_ptr.hpp>

namespace{
    std::optional<GLuint> current_program = std::nullopt;
    std::map<GLuint, std::queue<std::function<void()>>> pending_uniforms;

    void setUniform(GLuint program, std::function<void()> &&set_function){
        if (current_program.has_value() && program == current_program.value()){
            set_function();
        }
        else{
            pending_uniforms[program].emplace(std::move(set_function));
        }
    }

    template <typename SequenceContainer, typename Func>
    requires requires(SequenceContainer c, Func func) {
        func(c.front()); // container must have front() method and its result type must be passed to func parameter.
        c.pop();         // container must have pop() method.
        c.empty();       // container must have empty() method.
    }
    void consumeAll(SequenceContainer &container, Func &&func){
        while (!container.empty()){
            func(container.front());
            container.pop();
        }
    }
};

std::optional<GLuint> OpenGL::State::getProgram(){
    return current_program;
}

bool OpenGL::State::setProgram(GLuint program) {
    if (!current_program.has_value() || current_program.value() != program){
        current_program = program;
        glUseProgram(program);

        consumeAll(pending_uniforms[program], [] (auto &&set_function) {
            set_function();
            // Note that the lifetime of captured uniform value must be remained until here.
        });
        return true;
    }
    return false;
}

void OpenGL::State::setUniform(GLuint program, GLint uniform_location, int value){
    ::setUniform(program, [=]() { glUniform1i(uniform_location, value); });
}

void OpenGL::State::setUniform(GLuint program, GLint uniform_location, unsigned int value){
    ::setUniform(program, [=]() { glUniform1ui(uniform_location, value); });
}

void OpenGL::State::setUniform(GLuint program, GLint uniform_location, float value){
    ::setUniform(program, [=]() { glUniform1f(uniform_location, value); });
}

void OpenGL::State::setUniform(GLuint program, GLint uniform_location, glm::ivec2 &&value){
    ::setUniform(program, [=]() { glUniform2iv(uniform_location, 1, glm::value_ptr(value)); });
}

void OpenGL::State::setUniform(GLuint program, GLint uniform_location, glm::uvec2 &&value){
    ::setUniform(program, [=]() { glUniform2uiv(uniform_location, 1, glm::value_ptr(value)); });
}

void OpenGL::State::setUniform(GLuint program, GLint uniform_location, glm::vec2 &&value){
    ::setUniform(program, [=]() { glUniform2fv(uniform_location, 1, glm::value_ptr(value)); });
}

void OpenGL::State::setUniform(GLuint program, GLint uniform_location, glm::ivec3 &&value){
    ::setUniform(program, [=]() { glUniform3iv(uniform_location, 1, glm::value_ptr(value)); });
}

void OpenGL::State::setUniform(GLuint program, GLint uniform_location, glm::uvec3 &&value){
    ::setUniform(program, [=]() { glUniform3uiv(uniform_location, 1, glm::value_ptr(value)); });
}

void OpenGL::State::setUniform(GLuint program, GLint uniform_location, glm::vec3 &&value){
    ::setUniform(program, [=]() { glUniform3fv(uniform_location, 1, glm::value_ptr(value)); });
}

void OpenGL::State::setUniform(GLuint program, GLint uniform_location, glm::ivec4 &&value){
    ::setUniform(program, [=]() { glUniform4iv(uniform_location, 1, glm::value_ptr(value)); });
}

void OpenGL::State::setUniform(GLuint program, GLint uniform_location, glm::uvec4 &&value){
    ::setUniform(program, [=]() { glUniform4uiv(uniform_location, 1, glm::value_ptr(value)); });
}

void OpenGL::State::setUniform(GLuint program, GLint uniform_location, glm::vec4 &&value){
    ::setUniform(program, [=]() { glUniform4fv(uniform_location, 1, glm::value_ptr(value)); });
}

void OpenGL::State::setUniform(GLuint program, GLint uniform_location, glm::mat3 &&value){
    ::setUniform(program, [=]() { glUniformMatrix3fv(uniform_location, 1, GL_FALSE, glm::value_ptr(value)); });
}

void OpenGL::State::setUniform(GLuint program, GLint uniform_location, glm::mat4 &&value){
    ::setUniform(program, [=]() { glUniformMatrix4fv(uniform_location, 1, GL_FALSE, glm::value_ptr(value)); });
}