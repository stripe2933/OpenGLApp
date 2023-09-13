#version 330 core

in vec2 texCoords;

out vec4 FragColor;

uniform sampler2D material_texture;

void main(){
    FragColor = texture(material_texture, texCoords);
}