#version 330

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexNormal;
layout (location = 2) in vec2 VertexTexCoord;

out vec3 Position;
out vec3 Normal;
out vec2 TexCoord_1;
out vec2 TexCoord_2;

uniform float time;

uniform mat4 ModelViewMatrix;
uniform mat3 NormalMatrix;
uniform mat4 ProjectionMatrix;
uniform mat4 MVP;

void main()
{
mat2 m = mat2( vec2(cos(time), sin(time)), vec2(-sin(time), cos(time)) );
    TexCoord_1 = VertexTexCoord*m;
TexCoord_2 = VertexTexCoord*transpose(m);

    Normal = normalize( NormalMatrix * VertexNormal);
    Position = vec3( ModelViewMatrix * vec4(VertexPosition,1.0) );

    gl_Position = MVP * vec4(VertexPosition,1.0);
}
