//
// Created by gomkyung2 on 2023/08/29.
//

// You can adjust perspective matrix automatically, using onFramebufferSizeChanged() callback.

#include <OpenGLApp/Window.hpp>
#include <OpenGLApp/Program.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct Vertex{
    glm::vec3 position;
    glm::vec3 normal;
};
static_assert(std::is_standard_layout_v<Vertex>);

class App : public OpenGL::Window{
private:
    static constexpr std::array<Vertex, 36> vertices {
        Vertex { { -0.5f, -0.5f, -0.5f }, {  0.0f,  0.0f, -1.0f } },
        Vertex { {  0.5f, -0.5f, -0.5f }, {  0.0f,  0.0f, -1.0f } },
        Vertex { {  0.5f,  0.5f, -0.5f }, {  0.0f,  0.0f, -1.0f } },
        Vertex { {  0.5f,  0.5f, -0.5f }, {  0.0f,  0.0f, -1.0f } },
        Vertex { { -0.5f,  0.5f, -0.5f }, {  0.0f,  0.0f, -1.0f } },
        Vertex { { -0.5f, -0.5f, -0.5f }, {  0.0f,  0.0f, -1.0f } },

        Vertex { { -0.5f, -0.5f,  0.5f }, {  0.0f,  0.0f,  1.0f } },
        Vertex { {  0.5f, -0.5f,  0.5f }, {  0.0f,  0.0f,  1.0f } },
        Vertex { {  0.5f,  0.5f,  0.5f }, {  0.0f,  0.0f,  1.0f } },
        Vertex { {  0.5f,  0.5f,  0.5f }, {  0.0f,  0.0f,  1.0f } },
        Vertex { { -0.5f,  0.5f,  0.5f }, {  0.0f,  0.0f,  1.0f } },
        Vertex { { -0.5f, -0.5f,  0.5f }, {  0.0f,  0.0f,  1.0f } },

        Vertex { { -0.5f,  0.5f,  0.5f }, { -1.0f,  0.0f,  0.0f } },
        Vertex { { -0.5f,  0.5f, -0.5f }, { -1.0f,  0.0f,  0.0f } },
        Vertex { { -0.5f, -0.5f, -0.5f }, { -1.0f,  0.0f,  0.0f } },
        Vertex { { -0.5f, -0.5f, -0.5f }, { -1.0f,  0.0f,  0.0f } },
        Vertex { { -0.5f, -0.5f,  0.5f }, { -1.0f,  0.0f,  0.0f } },
        Vertex { { -0.5f,  0.5f,  0.5f }, { -1.0f,  0.0f,  0.0f } },

        Vertex { {  0.5f,  0.5f,  0.5f }, {  1.0f,  0.0f,  0.0f } },
        Vertex { {  0.5f,  0.5f, -0.5f }, {  1.0f,  0.0f,  0.0f } },
        Vertex { {  0.5f, -0.5f, -0.5f }, {  1.0f,  0.0f,  0.0f } },
        Vertex { {  0.5f, -0.5f, -0.5f }, {  1.0f,  0.0f,  0.0f } },
        Vertex { {  0.5f, -0.5f,  0.5f }, {  1.0f,  0.0f,  0.0f } },
        Vertex { {  0.5f,  0.5f,  0.5f }, {  1.0f,  0.0f,  0.0f } },

        Vertex { { -0.5f, -0.5f, -0.5f }, {  0.0f, -1.0f,  0.0f } },
        Vertex { {  0.5f, -0.5f, -0.5f }, {  0.0f, -1.0f,  0.0f } },
        Vertex { {  0.5f, -0.5f,  0.5f }, {  0.0f, -1.0f,  0.0f } },
        Vertex { {  0.5f, -0.5f,  0.5f }, {  0.0f, -1.0f,  0.0f } },
        Vertex { { -0.5f, -0.5f,  0.5f }, {  0.0f, -1.0f,  0.0f } },
        Vertex { { -0.5f, -0.5f, -0.5f }, {  0.0f, -1.0f,  0.0f } },

        Vertex { { -0.5f,  0.5f, -0.5f }, {  0.0f,  1.0f,  0.0f } },
        Vertex { {  0.5f,  0.5f, -0.5f }, {  0.0f,  1.0f,  0.0f } },
        Vertex { {  0.5f,  0.5f,  0.5f }, {  0.0f,  1.0f,  0.0f } },
        Vertex { {  0.5f,  0.5f,  0.5f }, {  0.0f,  1.0f,  0.0f } },
        Vertex { { -0.5f,  0.5f,  0.5f }, {  0.0f,  1.0f,  0.0f } },
        Vertex { { -0.5f,  0.5f, -0.5f }, {  0.0f,  1.0f,  0.0f } }
    };

    OpenGL::Program program;
    glm::mat4 model, view, projection;
    struct{
        GLuint vao;
        GLuint vbo;
    } cube;

    void onFramebufferSizeChanged(int width, int height) override {
        OpenGL::Window::onFramebufferSizeChanged(width, height); // Call base class method.

        projection = glm::perspective(glm::radians(45.0f), getAspectRatio(), 0.1f, 100.0f);
        program.setUniform("projection_view", projection * view);
    }

    void update(float time_delta) override {
        model = glm::rotate(model, time_delta, glm::vec3(0.0f, 1.0f, 0.0f));
        program.setUniform("model", model);
        program.setUniform("inv_model", glm::inverse(model));
    }

    void draw() const override {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        program.use();
        glBindVertexArray(cube.vao);
        glDrawArrays(GL_TRIANGLES, 0, vertices.size());
    }

public:
    App() : Window { 800, 480, "Hello Triangle" },
            program { "shaders/rotating_cube.vert", "shaders/rotating_cube.frag" }
    {
        constexpr glm::vec3 camera_pos { 3.f };

        model = glm::identity<glm::mat4>();
        view = glm::lookAt(camera_pos, glm::vec3(0.f), glm::vec3(0.0f, 1.0f, 0.0f));
        projection = glm::perspective(glm::radians(45.0f), getAspectRatio(), 0.1f, 100.0f);
        program.setUniform("model", model);
        program.setUniform("inv_model", glm::inverse(model));
        program.setUniform("projection_view", projection * view);

        program.setUniform("view_pos", camera_pos);

        program.setUniform("light.position", glm::vec3(0.f, 0.f, 3.f));
        program.setUniform("light.ambient", glm::vec3(0.1f));
        program.setUniform("light.diffuse", glm::vec3(1.f));
        program.setUniform("light.specular", glm::vec3(1.f));
        program.setUniform("light.constant",  1.0f);
        program.setUniform("light.linear",    0.35f);
        program.setUniform("light.quadratic", 0.44f);

        program.setUniform("material.ambient", glm::vec3(1.f, 0.5f, 0.31f));
        program.setUniform("material.diffuse", glm::vec3(1.f, 0.5f, 0.31f));
        program.setUniform("material.specular", glm::vec3(0.5f));
        program.setUniform("material.shininess", 32.f);

        glEnable(GL_DEPTH_TEST);

        glGenVertexArrays(1, &cube.vao);
        glBindVertexArray(cube.vao);

        glGenBuffers(1, &cube.vbo);
        glBindBuffer(GL_ARRAY_BUFFER, cube.vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<const GLint*>(offsetof(Vertex, position)));
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<const GLint*>(offsetof(Vertex, normal)));
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