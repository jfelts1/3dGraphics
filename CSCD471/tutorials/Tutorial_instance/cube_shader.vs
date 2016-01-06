#version 330 core

layout (location = 0) in vec4 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec4 instance_color;
layout (location = 3) in mat4 model_matrix;


out Fragment
{
    vec3 normal;
    vec4 color;
} fragment;

void main(void)
{
    gl_Position = (model_matrix*position)*vec4(0.15, 0.15, 0.15, 1.0);
fragment.normal = mat3(model_matrix)*normal;
fragment.color = instance_color;

}
