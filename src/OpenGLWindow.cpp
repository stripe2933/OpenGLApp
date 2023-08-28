//
// Created by gomkyung2 on 2023/08/29.
//

#include "OpenGLWindow.hpp"

#include <stdexcept>

GLFWwindow *createGlfwWindow(int width, int height, const char *title){
    if(!glfwInit()){
        throw std::runtime_error("Failed to initialize GLFW");
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow* window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if(!window){
        glfwTerminate();
        throw std::runtime_error("Failed to create GLFW window");
    }

    return window;
}

void OpenGLWindow::onFramebufferSizeChanged(int width, int height) {
    glViewport(0, 0, width, height);
}

void OpenGLWindow::onKeyChanged(int key, int scancode, int action, int mods) {

}

void OpenGLWindow::onMouseButtonChanged(int button, int action, int mods) {

}

void OpenGLWindow::onCursorPosChanged(double xpos, double ypos) {

}

void OpenGLWindow::onScrollChanged(double xoffset, double yoffset) {

}

OpenGLWindow::OpenGLWindow(int width, int height, const char *title)
        : window { createGlfwWindow(width, height, title) }
{
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    if(glewInit() != GLEW_OK){
        throw std::runtime_error("Failed to initialize GLEW");
    }

    // Make callback can access the class instance using pointer.
    glfwSetWindowUserPointer(window, this);
    glfwSetFramebufferSizeCallback(window, [](GLFWwindow* window_ptr, int width, int height){
        auto app = static_cast<OpenGLWindow*>(glfwGetWindowUserPointer(window_ptr));
        app->onFramebufferSizeChanged(width, height);
    });
    glfwSetKeyCallback(window, [](GLFWwindow* window_ptr, int key, int scancode, int action, int mods){
        auto app = static_cast<OpenGLWindow*>(glfwGetWindowUserPointer(window_ptr));
        app->onKeyChanged(key, scancode, action, mods);
    });
    glfwSetMouseButtonCallback(window, [](GLFWwindow* window_ptr, int button, int action, int mods){
        auto app = static_cast<OpenGLWindow*>(glfwGetWindowUserPointer(window_ptr));
        app->onMouseButtonChanged(button, action, mods);
    });
    glfwSetCursorPosCallback(window, [](GLFWwindow* window_ptr, double xpos, double ypos){
        auto app = static_cast<OpenGLWindow*>(glfwGetWindowUserPointer(window_ptr));
        app->onCursorPosChanged(xpos, ypos);
    });
    glfwSetScrollCallback(window, [](GLFWwindow* window_ptr, double xoffset, double yoffset){
        auto app = static_cast<OpenGLWindow*>(glfwGetWindowUserPointer(window_ptr));
        app->onScrollChanged(xoffset, yoffset);
    });
}

OpenGLWindow::~OpenGLWindow() noexcept {
    glfwDestroyWindow(window);
    glfwTerminate();
}

void OpenGLWindow::run() {
    float elapsed_time = 0.f;
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        const auto time_delta = static_cast<float>(glfwGetTime()) - elapsed_time;
        elapsed_time += time_delta;

        update(time_delta);
        draw();
        glfwSwapBuffers(window);
    }
}
