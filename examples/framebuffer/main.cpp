//
// Created by gomkyung2 on 2023/09/13.
//

/**
 * It first draws a cube and floor to generated framebuffer and apply Gaussian blur filter to offscreen rendered texture
 * and present it into the screen framebuffer.
 * The original source is from LearnOpenGL, https://learnopengl.com/Advanced-OpenGL/Framebuffers .
 */

#include <OpenGLApp/Window.hpp>
#include <OpenGLApp/Program.hpp>
#include <OpenGLApp/Camera.hpp>
#include <OpenGLApp/Utils/Image.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../models.hpp"

struct GLObject{
    const GLuint vao;
    const GLuint vbo;

    GLObject() : vao { generateVertexArray() }, vbo { generateBuffer() }{

    }

    ~GLObject() noexcept{
        glDeleteVertexArrays(1, &vao);
        glDeleteBuffers(1, &vbo);
    }

    static GLuint generateVertexArray(){
        GLuint handle;
        glGenVertexArrays(1, &handle);
        return handle;
    }

    static GLuint generateBuffer(){
        GLuint handle;
        glGenBuffers(1, &handle);
        return handle;
    }
};

class App : public OpenGL::Window {
private:
    OpenGL::Program render_program, blur_program;
    GLObject cube, plane, quad;
    GLuint container_texture, metal_texture;
    GLuint fbo, texture_color_buffer, rbo;

    OpenGL::Camera camera;
    std::optional<glm::vec2> previous_mouse_position;
    const struct{
        float scroll_sensitivity = 0.1f;
        float pan_sensitivity = 3e-3f;
    } control;
    glm::mat4 model { glm::identity<glm::mat4>() }, view, projection;

    void onFramebufferSizeChanged(int width, int height) override {
        OpenGL::Window::onFramebufferSizeChanged(width, height);
        projection = camera.getProjection(getFramebufferAspectRatio());

        // If screen size is changed, framebuffer's color buffer and render buffer also should be regenerated.
        // Previous buffer should be destroyed, so pass the delete_previous parameter as true.
        generateRenderbuffer(true);
    }

    void onScrollChanged(double xoffset, double yoffset) override {
        constexpr float min_distance = 1.f;
        camera.distance = std::fmax(
            std::exp(control.scroll_sensitivity * static_cast<float>(-yoffset)) * camera.distance,
            min_distance
        );
        view = camera.getView();
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
    }

    void onCursorPosChanged(double xpos, double ypos) override {
        const glm::vec2 position { xpos, ypos };
        if (previous_mouse_position.has_value()) {
            const glm::vec2 offset = control.pan_sensitivity * (position - previous_mouse_position.value());
            previous_mouse_position = position;

            camera.addYaw(offset.x);
            camera.addPitch(-offset.y);

            view = camera.getView();
        }
    }

    void update(float time_delta) override {
        render_program.setUniform("model", model);
        render_program.setUniform("projection_view", projection * view);
        blur_program.setUniform("framebuffer_size", getFramebufferSize());
    }

    void draw() const override {
        // offscreen rendering (render to fbo).
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);
        glEnable(GL_DEPTH_TEST);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        render_program.use();

        render_program.setUniform("material_texture", 0);
        glBindVertexArray(cube.vao);
        glActiveTexture(GL_TEXTURE0);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        render_program.setUniform("material_texture", 1);
        glBindVertexArray(plane.vao);
        glActiveTexture(GL_TEXTURE1);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        // restore to default framebuffer and present the result into screen.
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glDisable(GL_DEPTH_TEST);
        glClear(GL_COLOR_BUFFER_BIT);

        blur_program.use();
        glBindVertexArray(quad.vao);
        glActiveTexture(GL_TEXTURE2);
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }

    void setObjects(){
        // Set cube vertices.
        glBindVertexArray(cube.vao);
        glBindBuffer(GL_ARRAY_BUFFER, cube.vbo);
        glBufferData(GL_ARRAY_BUFFER,
                     static_cast<GLsizei>(sizeof(VertexPT<3>) * models::tex_cube.size()),
                     models::tex_cube.data(),
                     GL_STATIC_DRAW);
        glVertexAttribPointer(0,
                              3,
                              GL_FLOAT,
                              GL_FALSE,
                              sizeof(VertexPT<3>),
                              reinterpret_cast<GLint*>(offsetof(VertexPT<3>, position)));
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1,
                              2,
                              GL_FLOAT,
                              GL_FALSE,
                              sizeof(VertexPT<3>),
                              reinterpret_cast<GLint*>(offsetof(VertexPT<3>, tex_coords)));
        glEnableVertexAttribArray(1);

        // Set plane vertices.
        glBindVertexArray(plane.vao);
        glBindBuffer(GL_ARRAY_BUFFER, plane.vbo);
        glBufferData(GL_ARRAY_BUFFER,
                     static_cast<GLsizei>(sizeof(VertexPT<3>) * models::plane.size()),
                     models::plane.data(),
                     GL_STATIC_DRAW);
        glVertexAttribPointer(0,
                              3,
                              GL_FLOAT,
                              GL_FALSE,
                              sizeof(VertexPT<3>),
                              reinterpret_cast<GLint*>(offsetof(VertexPT<3>, position)));
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1,
                              2,
                              GL_FLOAT,
                              GL_FALSE,
                              sizeof(VertexPT<3>),
                              reinterpret_cast<GLint*>(offsetof(VertexPT<3>, tex_coords)));
        glEnableVertexAttribArray(1);

        // Set quad vertices, which covers the full region of the screen.
        glBindVertexArray(quad.vao);
        glBindBuffer(GL_ARRAY_BUFFER, quad.vbo);
        glBufferData(GL_ARRAY_BUFFER,
                     static_cast<GLsizei>(sizeof(VertexPT<2>) * models::full_quad.size()),
                     models::full_quad.data(),
                     GL_STATIC_DRAW);
        glVertexAttribPointer(0,
                              2,
                              GL_FLOAT,
                              GL_FALSE,
                              sizeof(VertexPT<2>),
                              reinterpret_cast<GLint*>(offsetof(VertexPT<2>, position)));
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1,
                              2,
                              GL_FLOAT,
                              GL_FALSE,
                              sizeof(VertexPT<2>),
                              reinterpret_cast<GLint*>(offsetof(VertexPT<2>, tex_coords)));
        glEnableVertexAttribArray(1);
    }

    void setTextures(){
        const OpenGL::Utils::Image container_img { "assets/container.jpg" };
        glGenTextures(1, &container_texture);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, container_texture);
        glTexImage2D(GL_TEXTURE_2D,
                     0,
                     GL_RGB,
                     container_img.width,
                     container_img.height,
                     0,
                     GL_RGB,
                     GL_UNSIGNED_BYTE,
                     container_img.data);
        glGenerateMipmap(GL_TEXTURE_2D);

        const OpenGL::Utils::Image metal_img { "assets/metal.png" };
        glGenTextures(1, &metal_texture);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, metal_texture);
        glTexImage2D(GL_TEXTURE_2D,
                     0,
                     GL_RGB,
                     metal_img.width,
                     metal_img.height,
                     0,
                     GL_RGB,
                     GL_UNSIGNED_BYTE,
                     metal_img.data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }

    void setFramebuffer(){
        glGenFramebuffers(1, &fbo);
        generateRenderbuffer(false);

        blur_program.setUniform("screen_texture", 2);
    }

    void generateRenderbuffer(bool delete_previous){
        if (delete_previous){
            glDeleteTextures(1, &texture_color_buffer);
            glDeleteRenderbuffers(1, &rbo);
        }

        // You should use framebuffer size, not window size.
        const auto framebuffer_size = getFramebufferSize();

        glBindFramebuffer(GL_FRAMEBUFFER, fbo);

        glGenTextures(1, &texture_color_buffer);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, texture_color_buffer);
        glTexImage2D(GL_TEXTURE_2D,
                     0,
                     GL_RGB,
                     framebuffer_size.x,
                     framebuffer_size.y,
                     0,
                     GL_RGB,
                     GL_UNSIGNED_BYTE, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture_color_buffer, 0);

        glGenRenderbuffers(1, &rbo);
        glBindRenderbuffer(GL_RENDERBUFFER, rbo);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, framebuffer_size.x, framebuffer_size.y);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

        assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
    }

public:
    App() : OpenGL::Window { 800, 480, "Framebuffer" },
            render_program { "shaders/framebuffer/render.vert", "shaders/framebuffer/render.frag" },
            blur_program { "shaders/framebuffer/gaussian_blur.vert", "shaders/framebuffer/gaussian_blur.frag" }
    {
        camera.distance = 5.f;
        camera.addPitch(-0.5f);

        view = camera.getView();
        projection = camera.getProjection(getFramebufferAspectRatio());

        setObjects();
        setTextures();
        setFramebuffer();

        glClearColor(0.f, 0.f, 0.f, 1.0f);
    }

    ~App() noexcept override{
        glDeleteRenderbuffers(1, &rbo);
        glDeleteFramebuffers(1, &fbo);
        glDeleteTextures(1, &container_texture);
        glDeleteTextures(1, &metal_texture);
        glDeleteTextures(1, &texture_color_buffer);
    }
};

int main() {
    App{}.run();
}
