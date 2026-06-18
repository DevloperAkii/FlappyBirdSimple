#version 460

out vec4 FragColor;
in vec2 UV;

uniform sampler2D u_BaseTexture;

void main()
{
    FragColor = texture(u_BaseTexture, UV);
}