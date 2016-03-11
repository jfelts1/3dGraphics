#version 330 
layout (triangles) in;
layout (line_strip, max_vertices = 2) out;

in VS_OUT {
    vec3 color;
    vec2 TexCoord;
    vec3 viewDir;
    vec3 lightDir;
} gs_in[];

in vec3 Normal[];
out vec2 TexCoord;
out vec3 viewDir;
out vec3 lightDir;


out vec3 fColor;
void main() 
{  
    for(int i =0;i<3;i++)
    {
        TexCoord = gs_in[i].TexCoord;
        viewDir = gs_in[i].viewDir;
        lightDir = gs_in[i].lightDir;
        fColor = gs_in[i].color;

        gl_Position = gl_in[i].gl_Position;
        EmitVertex();
        vec4 offset = vec4(Normal[i],0.0);
        offset/=3.0f;//reduce the commically long hairs of the bunny to make them look more reasonable
        gl_Position = gl_in[i].gl_Position + offset;
        EmitVertex();
        EndPrimitive();
    }


} 
