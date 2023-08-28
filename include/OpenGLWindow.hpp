//
// Created by gomkyung2 on 2023/08/29.
//

#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class OpenGLWindow{
protected:
    GLFWwindow* const window;

    virtual void update(float time_delta) = 0;
    virtual void draw() const = 0;

    virtual void onFramebufferSizeChanged(int width, int height);
    virtual void onKeyChanged(int key, int scancode, int action, int mods);
    virtual void onMouseButtonChanged(int button, int action, int mods);
    virtual void onCursorPosChanged(double xpos, double ypos);
    virtual void onScrollChanged(double xoffset, double yoffset);

public:
    OpenGLWindow(int width, int height, const char *title);
    virtual ~OpenGLWindow() noexcept;

    void run();
};