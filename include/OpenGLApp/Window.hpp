//
// Created by gomkyung2 on 2023/08/29.
//

#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/ext/vector_uint2.hpp>

namespace OpenGL{
    class Window{
    private:
        glm::uvec2 size;
        glm::vec<2, GLsizei> framebuffer_size;

    protected:
        GLFWwindow* const window;

        virtual void update(float time_delta) = 0;
        virtual void draw() const = 0;

        virtual void onWindowSizeChanged(int width, int height);
        virtual void onFramebufferSizeChanged(int width, int height);
        virtual void onKeyChanged(int key, int scancode, int action, int mods);
        virtual void onMouseButtonChanged(int button, int action, int mods);
        virtual void onCursorPosChanged(double xpos, double ypos);
        virtual void onScrollChanged(double xoffset, double yoffset);

    public:
        Window(int width, int height, const char *title);
        virtual ~Window() noexcept;

        void run();

        [[nodiscard]] glm::uvec2 getSize() const noexcept;
        [[nodiscard]] glm::vec<2, GLsizei> getFramebufferSize() const noexcept;
        [[nodiscard]] float getAspectRatio() const noexcept;
    };
};