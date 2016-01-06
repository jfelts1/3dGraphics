#version 330 core

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


void main() {
    vec3 ambient = Spot.intensity * Ka;

    FragColor = vec4(ambient, 1.0);
}


