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

    // Get the position and normal in eye space
    vec3 norm = normalize(NormalMatrix*VertexNormal);
    vec4 position = ModelViewMatrix*vec4(VertexPosition,1.0);
        
    vec3 ambient = Light.La * Material.Ka;
    vec4 viewDirection = normalize(-position);
    vec4 lightDirection = normalize(Light.Position-position);

   // Calculate diffuse and specular reflection and add to the light intensity

    vec3 diffuseReflect = Material.Kd*Light.Ld*dot(norm,vec3(lightDirection));
    vec3 specularReflect;

    if(diffuseReflect != vec3(0.0))
    {
        vec3 H = vec3((lightDirection)/normalize(lightDirection));
        //float angle = acos(dot(norm,H)/(length(norm)*length(H)));
        specularReflect = Material.Ks*Light.Ls*pow(max(dot(norm,H),0.0),Material.Shininess);
    }
    else
    {
        specularReflect = vec3(0.0);
    }

    LightIntensity = ambient + diffuseReflect/* + specularReflect*/;


    gl_Position = MVP * vec4(VertexPosition,1.0);
}
