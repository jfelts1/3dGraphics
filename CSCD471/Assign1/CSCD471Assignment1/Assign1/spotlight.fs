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

    vec4 viewDirection = normalize(vec4(-Position,0.0));
    vec4 lightDirection = normalize(Spot.position-vec4(Position,0.0));
    
    vec3 diffuseReflect = Kd*Spot.intensity*max(dot(Normal,vec3(lightDirection)),0.0);
    vec3 specularReflect;

    if(diffuseReflect != vec3(0.0))
    {
        vec3 H = vec3(normalize(lightDirection+viewDirection));
        specularReflect = Ks*Spot.intensity*pow(max(dot(Normal,H),0.0),Shininess);
    }
    else
    {
        specularReflect = vec3(0.0);
    }
    
    vec3 LightIntensity = diffuseReflect + specularReflect;
    
    vec3 ambient = Spot.intensity * Ka;
    float spotCos = dot(Spot.direction,-Position);
    float atten = 1.0;
    
    if(spotCos<Spot.cutoff)
    {
        FragColor = vec4(ambient*LightIntensity, 1.0);
    }
    else
    {
        atten *= pow(spotCos,Spot.exponent);
        FragColor = vec4(ambient*atten*LightIntensity, 1.0);
    }
    
}


