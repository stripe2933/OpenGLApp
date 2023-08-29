cmake_minimum_required(VERSION 3.11)
project(OpenGLApp LANGUAGES CXX)

if (NOT DEFINED BUILD_EXAMPLE)
    set(BUILD_EXAMPLE OFF)
endif()

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)

add_library(OpenGLApp STATIC src/OpenGLApp/Window.cpp
        src/OpenGLApp/State.cpp
        src/OpenGLApp/Program.cpp
        src/OpenGLApp/Camera.cpp)
target_include_directories(OpenGLApp PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(OpenGLApp PUBLIC OpenGL::GL GLEW::GLEW glfw glm::glm)
target_compile_features(OpenGLApp PUBLIC cxx_std_20)

if (BUILD_EXAMPLE)
    add_subdirectory(example)
endif()