#version 330

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexNormal;
layout (location = 2) in vec2 VertexTexCoord;
layout (location = 3) in vec4 VertexTangent;

out vec3 Position;
out vec3 Normal;
out vec2 TexCoord;
out mat3 tangentSpace;

uniform mat4 ModelViewMatrix;
uniform mat3 NormalMatrix;
uniform mat4 ProjectionMatrix;
uniform mat4 MVP;

void main()
{
    TexCoord = VertexTexCoord;
    Normal = normalize( NormalMatrix * VertexNormal);
    
    vec3 tang = normalize(NormalMatrix*vec3(VertexTangent));
    vec3 binomal = normalize(cross(Normal,tang))*VertexTangent.w;
    
    tangentSpace = mat3(
    tang.x,binomal.x,Normal.x,
    tang.y,binomal.y,Normal.y,
    tang.z,binomal.z,Normal.z);
    
    Position = vec3( ModelViewMatrix * vec4(VertexPosition,1.0) );
    gl_Position = MVP * vec4(VertexPosition,1.0);
}
