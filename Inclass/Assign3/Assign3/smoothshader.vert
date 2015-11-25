#version 330

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexNormal;

out vec3 LightIntensity;
out vec3 fN;
out vec3 fE;
out vec3 fL;

struct LightInfo {
  vec4 Position; // Light position in eye coords.
  vec3 La;       // Ambient light intensity
  vec3 Ld;       // Diffuse light intensity
  vec3 Ls;       // Specular light intensity
};
uniform LightInfo Light;

struct MaterialInfo {
  vec3 Ka;            // Ambient reflectivity
  vec3 Kd;            // Diffuse reflectivity
  vec3 Ks;            // Specular reflectivity
  float Shininess;    // Specular shininess factor
};
uniform MaterialInfo Material;

uniform mat4 ModelViewMatrix;
uniform mat3 NormalMatrix;
uniform mat4 ProjectionMatrix;
uniform mat4 MVP;


void main()
{
    vec3 norm = normalize(NormalMatrix*VertexNormal);
    vec4 position = ModelViewMatrix*vec4(VertexPosition,1.0);

    // Get the position and normal in eye space
        
    vec3 ambient = Light.La * Material.Ka;

   // Calculate diffuse and specular reflection and add to the light intensity
        
    LightIntensity =  ambient;
    fN = VertexNormal;
    fE = VertexPosition;
    fL = Light.Position.xyz-VertexPosition;


    gl_Position = MVP * vec4(VertexPosition,1.0);
}
