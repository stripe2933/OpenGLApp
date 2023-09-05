//
// Created by gomkyung2 on 2023/08/29.
//

/*
 * Enter WASD to move the camera. It will move the target point.
 * Drag screen to rotate the camera. Camera will rotate around the target point.
 */

#include <OpenGLApp/Window.hpp>
#include <OpenGLApp/Program.hpp>
#include <OpenGLApp/Camera.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/ext/matrix_transform.hpp>

struct Vertex{
    glm::vec3 position;
    glm::vec3 color;
};

static_assert(std::is_standard_layout_v<Vertex>);

class App : public OpenGL::Window{
private:
    static constexpr std::array<Vertex, 36> vertices {
        Vertex { { -0.5f, -0.5f, -0.5f }, { 1.f, 0.f, 0.f } },
        Vertex { {  0.5f, -0.5f, -0.5f }, { 1.f, 0.f, 0.f } },
        Vertex { {  0.5f,  0.5f, -0.5f }, { 1.f, 0.f, 0.f } },
        Vertex { {  0.5f,  0.5f, -0.5f }, { 1.f, 0.f, 0.f } },
        Vertex { { -0.5f,  0.5f, -0.5f }, { 1.f, 0.f, 0.f } },
        Vertex { { -0.5f, -0.5f, -0.5f }, { 1.f, 0.f, 0.f } },

        Vertex { { -0.5f, -0.5f,  0.5f }, { 0.f, 1.f, 0.f } },
        Vertex { {  0.5f, -0.5f,  0.5f }, { 0.f, 1.f, 0.f } },
        Vertex { {  0.5f,  0.5f,  0.5f }, { 0.f, 1.f, 0.f } },
        Vertex { {  0.5f,  0.5f,  0.5f }, { 0.f, 1.f, 0.f } },
        Vertex { { -0.5f,  0.5f,  0.5f }, { 0.f, 1.f, 0.f } },
        Vertex { { -0.5f, -0.5f,  0.5f }, { 0.f, 1.f, 0.f } },

        Vertex { { -0.5f,  0.5f,  0.5f }, { 0.f, 0.f, 1.f } },
        Vertex { { -0.5f,  0.5f, -0.5f }, { 0.f, 0.f, 1.f } },
        Vertex { { -0.5f, -0.5f, -0.5f }, { 0.f, 0.f, 1.f } },
        Vertex { { -0.5f, -0.5f, -0.5f }, { 0.f, 0.f, 1.f } },
        Vertex { { -0.5f, -0.5f,  0.5f }, { 0.f, 0.f, 1.f } },
        Vertex { { -0.5f,  0.5f,  0.5f }, { 0.f, 0.f, 1.f } },

        Vertex { {  0.5f,  0.5f,  0.5f }, { 1.f, 1.f, 0.f } },
        Vertex { {  0.5f,  0.5f, -0.5f }, { 1.f, 1.f, 0.f } },
        Vertex { {  0.5f, -0.5f, -0.5f }, { 1.f, 1.f, 0.f } },
        Vertex { {  0.5f, -0.5f, -0.5f }, { 1.f, 1.f, 0.f } },
        Vertex { {  0.5f, -0.5f,  0.5f }, { 1.f, 1.f, 0.f } },
        Vertex { {  0.5f,  0.5f,  0.5f }, { 1.f, 1.f, 0.f } },

        Vertex { { -0.5f, -0.5f, -0.5f }, { 0.f, 1.f, 1.f } },
        Vertex { {  0.5f, -0.5f, -0.5f }, { 0.f, 1.f, 1.f } },
        Vertex { {  0.5f, -0.5f,  0.5f }, { 0.f, 1.f, 1.f } },
        Vertex { {  0.5f, -0.5f,  0.5f }, { 0.f, 1.f, 1.f } },
        Vertex { { -0.5f, -0.5f,  0.5f }, { 0.f, 1.f, 1.f } },
        Vertex { { -0.5f, -0.5f, -0.5f }, { 0.f, 1.f, 1.f } },

        Vertex { { -0.5f,  0.5f, -0.5f }, { 1.f, 0.f, 1.f } },
        Vertex { {  0.5f,  0.5f, -0.5f }, { 1.f, 0.f, 1.f } },
        Vertex { {  0.5f,  0.5f,  0.5f }, { 1.f, 0.f, 1.f } },
        Vertex { {  0.5f,  0.5f,  0.5f }, { 1.f, 0.f, 1.f } },
        Vertex { { -0.5f,  0.5f,  0.5f }, { 1.f, 0.f, 1.f } },
        Vertex { { -0.5f,  0.5f, -0.5f }, { 1.f, 0.f, 1.f } }
    };

    std::optional<glm::vec2> previous_mouse_position;
    std::optional<glm::vec3> camera_velocity;

    const struct{
        float scroll_sensitivity = 0.1f;
        float pan_sensitivity = 3e-3f;
        float speed = 2.5f;
    } camera_properties;

    OpenGL::Program program;
    OpenGL::Camera camera;

    glm::mat4 model, view, projection;
    GLuint vao, vbo;

    void onFramebufferSizeChanged(int width, int height) override {
        OpenGL::Window::onFramebufferSizeChanged(width, height);
        onCameraChanged();
    }

    void onScrollChanged(double xoffset, double yoffset) override {
        camera.distance = std::fmax(
            std::exp(camera_properties.scroll_sensitivity * static_cast<float>(-yoffset)) * camera.distance,
            camera.min_distance
        );
        onCameraChanged();
    }

    void onMouseButtonChanged(int button, int action, int mods) override {
        if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS){
            double mouse_x, mouse_y;
            glfwGetCursorPos(window, &mouse_x, &mouse_y);

            previous_mouse_position = glm::vec2(mouse_x, mouse_y);
        }
        else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE){
            previous_mouse_position = std::nullopt;
        }
        onCameraChanged();
    }

    void onCursorPosChanged(double xpos, double ypos) override {
        const glm::vec2 position { xpos, ypos };
        if (previous_mouse_position.has_value()) {
            const glm::vec2 offset = camera_properties.pan_sensitivity * (position - previous_mouse_position.value());
            previous_mouse_position = position;

            camera.addYaw(offset.x);
            camera.addPitch(-offset.y);

            onCameraChanged();
        }
    }

    void onKeyChanged(int key, int scancode, int action, int mods) override {
        if (action == GLFW_PRESS){
            const auto camera_front = camera.getFront();
            const auto camera_right = camera.getRight();
            const auto camera_up = camera.getUp();

            switch (key){
                case GLFW_KEY_A:
                    camera_velocity = camera_properties.speed * -camera_right;
                    break;
                case GLFW_KEY_D:
                    camera_velocity = camera_properties.speed * camera_right;
                    break;
                case GLFW_KEY_S:
                    camera_velocity = camera_properties.speed * -camera_up;
                    break;
                case GLFW_KEY_W:
                    camera_velocity = camera_properties.speed * camera_up;
                    break;
                default:
                    break;
            }
        }
        else if (action == GLFW_RELEASE && camera_velocity.has_value()){
            camera_velocity = std::nullopt;
        }
    }

    void update(float time_delta) override {
        if (camera_velocity.has_value()){
            camera.target += time_delta * camera_velocity.value();
            onCameraChanged();
        }
    }

    void draw() const override {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        program.use();
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, vertices.size());
    }

    void onCameraChanged(){
        view = camera.getView();
        projection = camera.getProjection(getAspectRatio());
        program.setUniform("projection_view", projection * view);
    }

public:
    App() : Window { 800, 480, "Targeting Camera" },
            program { "shaders/targeting_camera.vert", "shaders/targeting_camera.frag" }
    {
        camera.distance = 5.f;

        model = glm::identity<glm::mat4>();
        view = camera.getView();
        projection = camera.getProjection(getAspectRatio());

        program.setUniform("model", model);
        program.setUniform("projection_view", projection * view);

        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<GLint*>(offsetof(Vertex, position)));
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<GLint*>(offsetof(Vertex, color)));
        glEnableVertexAttribArray(1);

        glEnable(GL_DEPTH_TEST);
    }

    ~App() noexcept override{
        glDeleteVertexArrays(1, &vao);
        glDeleteBuffers(1, &vbo);
    }
};

int main(){
    App{}.run();
}