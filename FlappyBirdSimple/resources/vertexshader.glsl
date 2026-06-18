#version 460

layout(location = 0) in vec3 aPos;

uniform mat4 projMat;

void main()
{
    gl_Position = projMat * vec4(aPos, 1.0f);
}