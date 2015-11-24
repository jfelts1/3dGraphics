#version 330 core

subroutine vec3 shadeModelType( vec4 position, vec3 normal);
subroutine uniform shadeModelType shadeModel;


layout (location = 0) in vec4 position;
layout (location = 1) in vec3 normal;

out vec4 fColor;

uniform mat4 model_view;
uniform mat4 projection;
uniform vec4 LightPosition;

uniform vec4 AmbientProduct, DiffuseProduct, SpecularProduct;
uniform float Shininess;

subroutine( shadeModelType )
vec3 phongModel( vec4 position, vec3 norm )
{
    vec3 N = normalize(norm);
vec3 E = normalize(position.xyz);
vec3 L = normalize(LightPosition.xyz);

vec3 H = normalize(L+E);

float Kd = max(dot(L,N), 0.0);
vec4 diffuse = Kd*DiffuseProduct;

float Ks = pow(max(dot(N, H), 0.0), Shininess);
vec4 specular = Ks*SpecularProduct;

//discard the specular highlight if the light is behind the vertex

if (dot(L, N) < 0.0){
    specular = vec4 (0.0, 0.0, 0.0, 1.0);
}

    return vec3(AmbientProduct+diffuse + specular);
}

subroutine( shadeModelType )
vec3 diffuseOnly( vec4 position, vec3 norm )
{
   vec3 N = normalize(norm);
vec3 E = normalize(position.xyz);
vec3 L = normalize(LightPosition.xyz);


float Kd = max(dot(L,N), 0.0);
vec4 diffuse = Kd*DiffuseProduct;


 
    return vec3(AmbientProduct+diffuse);
}



void main(void)
{


fColor.rgb = shadeModel( position, normal );
fColor.a = 1.0;
   
gl_Position = (projection*model_view*position);


}

