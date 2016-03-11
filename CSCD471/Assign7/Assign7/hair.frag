#version 330

struct LightInfo {
  vec4 Position;  // Light position in eye coords.
  vec3 Intensity; // A,D,S intensity
};
uniform LightInfo Light;

in vec3 Position;
in vec3 Normal;
in vec2 TexCoord;
in vec3 viewDir;
in vec3 lightDir;
in vec3 fColor;

uniform sampler2D Tex1;
uniform sampler2D NormalMap;

struct MaterialInfo {
  vec3 Ka;            // Ambient reflectivity
  vec3 Kd;            // Diffuse reflectivity
  vec3 Ks;            // Specular reflectivity
  float Shininess;    // Specular shininess factor
};
uniform MaterialInfo Material;

layout( location = 0 ) out vec4 FragColor;

void phongModel( vec3 pos, vec3 norm, out vec3 ambAndDiff, out vec3 spec ) {
    vec3 s = lightDir;
    vec3 v = viewDir;
    vec3 r = reflect( -s, norm );
    vec3 ambient = Light.Intensity * Material.Ka;
    float sDotN = max( dot(s,norm), 0.0 );
    vec3 diffuse = Light.Intensity * Material.Kd * sDotN;
    spec = vec3(0.0);
    if( sDotN > 0.0 )
        spec = Light.Intensity * Material.Ks *
               pow( max( dot(r,v), 0.0 ), Material.Shininess );
    ambAndDiff = ambient + diffuse;
}

void main() {
    vec3 ambAndDiff, spec;
    vec4 texColor = texture( Tex1, TexCoord );
    vec4 normal = texture(NormalMap,TexCoord);
    normal = vec4(normalize(vec3(2.0*normal-1.0)),1.0f);
    phongModel(texColor.rgb, normal.xyz, ambAndDiff, spec );
    FragColor = (vec4( ambAndDiff, 1.0 ) * texColor) + vec4(spec,1.0);
}
