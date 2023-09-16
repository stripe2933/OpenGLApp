//
// Created by gomkyung2 on 2023/08/29.
//

/*
 * Enter WASD to move the camera. It will move the target point.
 * Drag screen to rotate the camera. Camera will rotate around the target point.
 */

#include "OpenGLApp/Window.hpp"
#include "OpenGLApp/Program.hpp"
#include "OpenGLApp/Camera.hpp"
#include <glm/gtc/constants.hpp>
#include <glm/ext/matrix_transform.hpp>

#include "../models.hpp"

class App : public OpenGL::Window{
private:
    std::optional<glm::vec2> previous_mouse_position;
    std::optional<glm::vec3> camera_velocity;

    const struct{
        float scroll_sensitivity = 0.1f;
        float pan_sensitivity = 3e-3f;
        float speed = 2.5f;
    } camera_properties;

    OpenGL::Program render_program;
    OpenGL::Camera camera;

    glm::mat4 model, view, projection;
    GLuint vao, vbo;

    void onFramebufferSizeChanged(int width, int height) override {
        OpenGL::Window::onFramebufferSizeChanged(width, height);
        onCameraChanged();
    }

    void onScrollChanged(double xoffset, double yoffset) override {
        constexpr float min_distance = 1.f;
        camera.distance = std::fmax(
            std::exp(camera_properties.scroll_sensitivity * static_cast<float>(-yoffset)) * camera.distance,
            min_distance
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

        render_program.use();
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(models::colored_cube.size()));
    }

    void onCameraChanged(){
        view = camera.getView();
        projection = camera.getProjection(getAspectRatio());
        render_program.setUniform("projection_view", projection * view);
    }

public:
    App() : Window { 800, 480, "Targeting Camera" },
            render_program { "shaders/targeting_camera/vert.vert", "shaders/targeting_camera/frag.frag" }
    {
        camera.distance = 5.f;

        model = glm::identity<glm::mat4>();
        view = camera.getView();
        projection = camera.getProjection(getAspectRatio());

        render_program.setUniform("model", model);
        render_program.setUniform("projection_view", projection * view);

        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER,
                     static_cast<GLsizei>(sizeof(VertexPC<3>) * models::colored_cube.size()),
                     models::colored_cube.data(),
                     GL_STATIC_DRAW);

        glVertexAttribPointer(0,
                              3,
                              GL_FLOAT,
                              GL_FALSE,
                              sizeof(VertexPC<3>),
                              reinterpret_cast<GLint*>(offsetof(VertexPC<3>, position)));
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1,
                              3,
                              GL_FLOAT,
                              GL_FALSE,
                              sizeof(VertexPC<3>),
                              reinterpret_cast<GLint*>(offsetof(VertexPC<3>, color)));
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