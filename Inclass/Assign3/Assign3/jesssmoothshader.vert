#version 430

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
    vec3 norm = normalize(NormalMatrix * VertexNormal);
    vec4 position = ModelViewMatrix * vec4(VertexPosition,1.0);

    // Get the position and normal in eye space
    
    vec3 ambient = Light.La * Material.Ka;
	

   // Calculate diffuse and specular reflection and add to the light intensity
    vec4 view_direction = normalize(-position); //bottom slide lec1_wk8
	vec4 light_direction = normalize(Light.Position - position);

	vec3 H = normalize(vec3(light_direction) + vec3(view_direction));//slide 49 lec2_wk7

	vec3 diffuse = Material.Kd * Light.Ld * max(dot(norm,vec3(light_direction)), 0.0);

	vec3 specular;
	if(diffuse == vec3(0.0))specular = vec3(0.0);
	else
	{
		specular = Material.Ks * Light.Ls * pow(max(dot(norm,H), 0.0),Material.Shininess);
			//pow(dot(norm,H),Material.Shininess); //specular reflection coefficient * Intensity of light source * cos(angle between viewer and reflection of light source) ^ glossiness of material 
    }

	LightIntensity = ambient + diffuse;// + specular; //defined lec 1_8 slide 3
	
	gl_Position = MVP * vec4(VertexPosition,1.0);
	
}
