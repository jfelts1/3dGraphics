#version 330

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexNormal;

out vec3 LightIntensity;

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
    vec3 viewDirection = -VertexPosition;

   // Calculate diffuse and specular reflection and add to the light intensity
        
    LightIntensity =  ambient;
    vec3 diffuseReflect = Material.Kd*LightIntensity*dot(VertexNormal,vec3(Light.Position));
    vec3 H = normalize(viewDirection+vec3(Light.Position));
    float angle = acos(dot(VertexNormal,H)/(length(VertexNormal)*length(H)));
    vec3 specularReflect = Material.Ks*pow(cos(angle),Material.Shininess);
    LightIntensity = ambient + diffuseReflect + specularReflect;


    gl_Position = MVP * vec4(VertexPosition,1.0);
}
