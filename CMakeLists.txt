cmake_minimum_required(VERSION 3.11)
project(OpenGLApp LANGUAGES CXX)

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glfw3 REQUIRED)

add_library(OpenGLApp STATIC src/OpenGLWindow.cpp)
target_include_directories(OpenGLApp PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(OpenGLApp PUBLIC OpenGL::GL GLEW::GLEW glfw)
target_compile_features(OpenGLApp PUBLIC cxx_std_20)

add_subdirectory(example)