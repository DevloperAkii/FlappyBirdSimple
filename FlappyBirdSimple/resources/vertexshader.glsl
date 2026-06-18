#version 460

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aUV;

uniform mat4 u_ProjMat;
uniform mat4 u_ModelMat;
out vec2 UV;

void main()
{
    UV = aUV;
    gl_Position = u_ProjMat * u_ModelMat * vec4(aPos, 1.0f);
}