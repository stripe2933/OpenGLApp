#version 330 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aColor;

out vec3 color;

uniform mat4 model;
uniform mat4 projection_view;

void main() {
    color = aColor;
    gl_Position = projection_view * model * vec4(aPosition, 1.0);
}
