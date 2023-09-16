//
// Created by gomkyung2 on 2023/08/29.
//

// You can adjust perspective matrix automatically, using onFramebufferSizeChanged() callback.

#include "OpenGLApp/Window.hpp"
#include "OpenGLApp/Program.hpp"

#include <glm/gtc/matrix_transform.hpp>

#include "../models.hpp"

class App : public OpenGL::Window{
private:
    OpenGL::Program render_program;
    glm::mat4 model, view, projection;
    struct{
        GLuint vao;
        GLuint vbo;
    } cube;

    void onFramebufferSizeChanged(int width, int height) override {
        OpenGL::Window::onFramebufferSizeChanged(width, height); // Call base class method.

        projection = glm::perspective(glm::radians(45.0f), getAspectRatio(), 0.1f, 100.0f);
        render_program.setUniform("projection_view", projection * view);
    }

    void update(float time_delta) override {
        model = glm::rotate(model, time_delta, glm::vec3(0.0f, 1.0f, 0.0f));
        render_program.setUniform("model", model);
        render_program.setUniform("inv_model", glm::inverse(model));
    }

    void draw() const override {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        render_program.use();
        glBindVertexArray(cube.vao);
        glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(models::normal_cube.size()));
    }

public:
    App() : Window { 800, 480, "Hello Triangle" },
            render_program { "shaders/rotating_cube/vert.vert", "shaders/rotating_cube/frag.frag" }
    {
        constexpr glm::vec3 camera_pos { 3.f };

        model = glm::identity<glm::mat4>();
        view = glm::lookAt(camera_pos, glm::vec3(0.f), glm::vec3(0.0f, 1.0f, 0.0f));
        projection = glm::perspective(glm::radians(45.0f), getAspectRatio(), 0.1f, 100.0f);
        render_program.setUniform("model", model);
        render_program.setUniform("inv_model", glm::inverse(model));
        render_program.setUniform("projection_view", projection * view);

        render_program.setUniform("view_pos", camera_pos);

        render_program.setUniform("light.position", glm::vec3(0.f, -1.f, 2.f));
        render_program.setUniform("light.ambient", glm::vec3(0.1f));
        render_program.setUniform("light.diffuse", glm::vec3(1.f));
        render_program.setUniform("light.specular", glm::vec3(1.f));
        render_program.setUniform("light.constant", 1.0f);
        render_program.setUniform("light.linear", 0.02f);
        render_program.setUniform("light.quadratic", 1.7e-3f);

        render_program.setUniform("material.ambient", glm::vec3(1.f, 0.5f, 0.31f));
        render_program.setUniform("material.diffuse", glm::vec3(1.f, 0.5f, 0.31f));
        render_program.setUniform("material.specular", glm::vec3(0.5f));
        render_program.setUniform("material.shininess", 32.f);

        glEnable(GL_DEPTH_TEST);

        glGenVertexArrays(1, &cube.vao);
        glBindVertexArray(cube.vao);

        glGenBuffers(1, &cube.vbo);
        glBindBuffer(GL_ARRAY_BUFFER, cube.vbo);
        glBufferData(GL_ARRAY_BUFFER,
                     static_cast<GLsizei>(sizeof(VertexPN<3>) * models::normal_cube.size()),
                     models::normal_cube.data(),
                     GL_STATIC_DRAW);
        glVertexAttribPointer(0,
                              3,
                              GL_FLOAT,
                              GL_FALSE,
                              sizeof(VertexPN<3>),
                              reinterpret_cast<const GLint*>(offsetof(VertexPN<3>, position)));
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1,
                              3,
                              GL_FLOAT,
                              GL_FALSE,
                              sizeof(VertexPN<3>),
                              reinterpret_cast<const GLint*>(offsetof(VertexPN<3>, normal)));
        glEnableVertexAttribArray(1);
    }

    ~App() noexcept override{
        glDeleteVertexArrays(1, &cube.vao);
        glDeleteBuffers(1, &cube.vbo);
    }
};

int main(){
    App{}.run();
}