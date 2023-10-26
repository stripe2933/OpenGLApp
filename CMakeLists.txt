cmake_minimum_required(VERSION 3.11)
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

# Set OPENGLAPP_STANDALONE_BUILD flag if it is unset and project is top-level project.
if (NOT DEFINED OPENGLAPP_STANDALONE_BUILD)
    set(OPENGLAPP_STANDALONE_BUILD (CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR))
endif()

if (OPENGLAPP_STANDALONE_BUILD)
    add_subdirectory(examples)
endif()