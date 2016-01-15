#version 330

in vec3 Position;
in vec3 Normal;

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

layout( location = 0 ) out vec4 FragColor;


void main() 
{

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
    
    vec3 LightIntensity = diffuseReflect + specularReflect;
    
    vec3 ambient = Spot.intensity * Ka;
    float spotCos = acos(dot(normalize(Spot.direction),-lightDirection));
    float atten = 1.0;
    
    if(spotCos<Spot.cutoff)
    {
        FragColor = vec4(ambient+LightIntensity, 1.0);
    }
    else
    {
        atten *= pow(spotCos,Spot.exponent);
        FragColor = vec4(ambient+LightIntensity*atten, 1.0);
        
    }
    
}


