#version 330

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexNormal;

out vec3 LightIntensity;

struct SpotLight {
    vec4 position;   // Position in eye coords
    vec3 intensity;
    vec3 direction;  // Direction of the spotlight in eye coords.
    float exponent;  // Angular attenuation exponent
    float cutoff;    // Cutoff angle
};
uniform SpotLight Spot;

uniform vec3 Kd;            // Diffuse reflectivity
uniform vec3 Ka;            // Ambient reflectivity
uniform vec3 Ks;            // Specular reflectivity
uniform float Shininess;    // Specular shininess factor

uniform mat4 ModelViewMatrix;
uniform mat3 NormalMatrix;
uniform mat4 ProjectionMatrix;
uniform mat4 MVP;

void main() 
{

    vec3 Normal = normalize( NormalMatrix * VertexNormal);
    vec3 Position = vec3( ModelViewMatrix * vec4(VertexPosition,1.0) );
    
    vec3 viewDirection = normalize(-Position);
    vec3 lightDirection = normalize(vec3(Spot.position)-Position);
    
    vec3 diffuseReflect = Kd*Spot.intensity*max(dot(Normal,lightDirection),0.0);
    vec3 specularReflect;

    if(diffuseReflect != vec3(0.0))
    {
        vec3 H = normalize(lightDirection+viewDirection);
        specularReflect = Ks*Spot.intensity*pow(max(dot(Normal,H),0.0),Shininess);
    }
    else
    {
        specularReflect = vec3(0.0);
    }
     
    vec3 ambient = Spot.intensity * Ka;
    float spotCos = acos(dot(normalize(Spot.direction),-lightDirection));
    float atten = 1.0;
    
    if(spotCos<Spot.cutoff)
    {
        LightIntensity = ambient + diffuseReflect + specularReflect;
    }
    else
    {
        atten *= pow(spotCos,Spot.exponent);
        LightIntensity = ambient + (diffuseReflect + specularReflect)*atten;       
    }

    gl_Position = MVP * vec4(VertexPosition,1.0);
}
