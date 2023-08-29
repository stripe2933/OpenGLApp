cmake_minimum_required(VERSION 3.11)
project(OpenGLApp LANGUAGES CXX)

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)

add_library(OpenGLApp STATIC src/OpenGLApp/Window.cpp
        src/OpenGLApp/State.cpp
        src/OpenGLApp/Program.cpp
        include/OpenGLApp/Program.hpp)
target_include_directories(OpenGLApp PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(OpenGLApp PUBLIC OpenGL::GL GLEW::GLEW glfw glm::glm)
target_compile_features(OpenGLApp PUBLIC cxx_std_20)

add_subdirectory(example)