#version 330 core

#define KERNEL_SIZE 13

in vec2 texCoords;
out vec4 FragColor;

uniform sampler2D screen_texture;
uniform ivec2 framebuffer_size;

const float kernel[KERNEL_SIZE * KERNEL_SIZE] = float[](
    0.0023, 0.0029, 0.0035, 0.0040, 0.0044, 0.0047, 0.0048, 0.0047, 0.0044, 0.0040, 0.0035, 0.0029, 0.0023,
    0.0029, 0.0036, 0.0043, 0.0050, 0.0055, 0.0058, 0.0059, 0.0058, 0.0055, 0.0050, 0.0043, 0.0036, 0.0029,
    0.0035, 0.0043, 0.0052, 0.0059, 0.0066, 0.0070, 0.0071, 0.0070, 0.0066, 0.0059, 0.0052, 0.0043, 0.0035,
    0.0040, 0.0050, 0.0059, 0.0068, 0.0075, 0.0080, 0.0082, 0.0080, 0.0075, 0.0068, 0.0059, 0.0050, 0.0040,
    0.0044, 0.0055, 0.0066, 0.0075, 0.0083, 0.0088, 0.0090, 0.0088, 0.0083, 0.0075, 0.0066, 0.0055, 0.0044,
    0.0047, 0.0058, 0.0070, 0.0080, 0.0088, 0.0094, 0.0096, 0.0094, 0.0088, 0.0080, 0.0070, 0.0058, 0.0047,
    0.0048, 0.0059, 0.0071, 0.0082, 0.0090, 0.0096, 0.0098, 0.0096, 0.0090, 0.0082, 0.0071, 0.0059, 0.0048,
    0.0047, 0.0058, 0.0070, 0.0080, 0.0088, 0.0094, 0.0096, 0.0094, 0.0088, 0.0080, 0.0070, 0.0058, 0.0047,
    0.0044, 0.0055, 0.0066, 0.0075, 0.0083, 0.0088, 0.0090, 0.0088, 0.0083, 0.0075, 0.0066, 0.0055, 0.0044,
    0.0040, 0.0050, 0.0059, 0.0068, 0.0075, 0.0080, 0.0082, 0.0080, 0.0075, 0.0068, 0.0059, 0.0050, 0.0040,
    0.0035, 0.0043, 0.0052, 0.0059, 0.0066, 0.0070, 0.0071, 0.0070, 0.0066, 0.0059, 0.0052, 0.0043, 0.0035,
    0.0029, 0.0036, 0.0043, 0.0050, 0.0055, 0.0058, 0.0059, 0.0058, 0.0055, 0.0050, 0.0043, 0.0036, 0.0029,
    0.0023, 0.0029, 0.0035, 0.0040, 0.0044, 0.0047, 0.0048, 0.0047, 0.0044, 0.0040, 0.0035, 0.0029, 0.0023
);

void main(){
    const int half_kernel_size = (KERNEL_SIZE - 1) / 2;

    vec3 convolution = vec3(0.0);
    for (int i = -half_kernel_size; i <= half_kernel_size; ++i){
        for (int j = -half_kernel_size; j <= half_kernel_size; ++j){
            int index = 3 * (i + half_kernel_size) + (j + half_kernel_size);
            convolution += texture(screen_texture, texCoords + vec2(j, i) / framebuffer_size).rgb * kernel[index];
        }
    }
    FragColor = vec4(convolution, 1.0);
}