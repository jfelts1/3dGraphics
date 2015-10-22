#version 330 core


layout (location = 0) in vec4 position;

uniform mat4 model_matrix;
uniform vec4 color;

out Fragment
{
    vec4 color;
} fragment;

void main(void)
{
    gl_Position = model_matrix*position * vec4(0.1, 0.1, 1.0, 1.0);
    fragment.color = color;
}
