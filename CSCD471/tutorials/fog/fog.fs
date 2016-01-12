#version 430

in vec3 Position;
in vec3 Normal;

uniform   vec4 LightPosition;
uniform   vec3 LightIntensity;

struct FogInfo {
  float maxDist;
  float minDist;
  vec3 color;
};
uniform FogInfo Fog;

uniform vec3 Kd;            // Diffuse reflectivity
uniform vec3 Ka;            // Ambient reflectivity
uniform vec3 Ks;            // Specular reflectivity
uniform float Shininess;    // Specular shininess factor

layout( location = 0 ) out vec4 FragColor;

vec3 ads( )
{
    vec3 s = normalize( LightPosition.xyz - Position.xyz );
    vec3 v = normalize(vec3(-Position));
    vec3 h = normalize( v + s );

    vec3 ambient = Ka;
    vec3 diffuse = Kd * max(0.0, dot(s, Normal) );
    vec3 spec = Ks * pow( max( 0.0, dot( h, Normal) ), Shininess );

    return LightIntensity * (ambient + diffuse + spec);
}

void main() {
    float dist = Position.z;
    float fogFactor = (Fog.maxDist - dist) /
                      (Fog.maxDist - Fog.minDist);
    fogFactor = clamp( fogFactor, 0.0, 1.0 );
    vec3 shadeColor = ads();
    vec3 color = mix( Fog.color, shadeColor, fogFactor );

    FragColor = vec4(color, 1.0);
}

