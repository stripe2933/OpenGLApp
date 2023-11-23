cmake_minimum_required(VERSION 3.24)
project(OpenGLApp LANGUAGES CXX)

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)
find_package(Stb REQUIRED)

add_library(OpenGLApp
    STATIC
        src/OpenGLApp/Window.cpp
        src/OpenGLApp/State.cpp
        src/OpenGLApp/Program.cpp
        src/OpenGLApp/Camera.cpp
        src/OpenGLApp/Shader.cpp
        src/OpenGLApp/Utils/Image.cpp
)
target_include_directories(OpenGLApp PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include ${Stb_INCLUDE_DIR})
target_link_libraries(OpenGLApp PUBLIC OpenGL::GL GLEW::GLEW glfw glm::glm)
target_compile_features(OpenGLApp PUBLIC cxx_std_20)

# Build examples when project is top-level project.
if (PROJECT_IS_TOP_LEVEL)
    add_subdirectory(examples)
endif()