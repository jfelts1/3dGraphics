#version 330

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexNormal;
layout (location = 2) in vec2 VertexTexCoord;
layout (location = 3) in vec4 VertexTangent;
layout (location = 4) in vec3 color;

struct LightInfo {
  vec4 Position;  // Light position in eye coords.
  vec3 Intensity; // A,D,S intensity
};
uniform LightInfo Light;

out VS_OUT {
    vec3 color;
    vec2 TexCoord;
    vec3 viewDir;
    vec3 lightDir;
} vs_out;

out vec3 Position;
out vec3 Normal;


uniform mat4 ModelViewMatrix;
uniform mat3 NormalMatrix;
uniform mat4 ProjectionMatrix;
uniform mat4 MVP;

void main()
{
    vs_out.TexCoord = VertexTexCoord;
    Normal = normalize( NormalMatrix * VertexNormal);
    
    vec3 tang = normalize(NormalMatrix*vec3(VertexTangent));
    vec3 binomal = normalize(cross(Normal,tang))*VertexTangent.w;
    
    mat3 tangentSpace = mat3(
    tang.x,binomal.x,Normal.x,
    tang.y,binomal.y,Normal.y,
    tang.z,binomal.z,Normal.z);

    Position = vec3( ModelViewMatrix * vec4(VertexPosition,1.0) );
    //Position *= tangentSpace;
    vs_out.viewDir = normalize(-Position.xyz)*tangentSpace;
    vs_out.lightDir = normalize(vec3(Light.Position) - Position)*tangentSpace;
    gl_Position = MVP * vec4(VertexPosition,1.0);
	//hair color is yellow
    vs_out.color = vec3(1.0,1.0,0.0);
}
