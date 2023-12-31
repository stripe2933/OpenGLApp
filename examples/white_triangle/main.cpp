//
// Created by gomkyung2 on 2023/08/29.
//

/*
 * It just draws a white triangle.
 */

#include "OpenGLApp/Window.hpp"
#include "OpenGLApp/Program.hpp"

class App : public OpenGL::Window{
private:
    static constexpr std::array<glm::vec2, 3> vertices {
        glm::vec2 { -0.5f, -0.5f },
        glm::vec2 { 0.5f, -0.5f },
        glm::vec2 { 0.0f,  0.5f }
    };

    OpenGL::Program render_program;
    GLuint vao, vbo;

    void update(float time_delta) override {

    }

    void draw() const override {
        glClear(GL_COLOR_BUFFER_BIT);

        render_program.use();
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 3);
    }

public:
    App() : Window { 800, 480, "Hello Triangle" },
            render_program { "shaders/white_triangle/vert.vert", "shaders/white_triangle/frag.frag" }
    {
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), nullptr);
        glEnableVertexAttribArray(0);
    }

    ~App() noexcept override{
        // render_program is automatically destroyed using RAII pattern.
        glDeleteVertexArrays(1, &vao);
        glDeleteBuffers(1, &vbo);
    }
};

int main(){
    App{}.run();
}
