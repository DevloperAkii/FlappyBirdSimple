#version 460

out vec4 FragColor;
in vec2 UV;

uniform vec3 u_Color;
uniform vec2 u_Tile;
uniform sampler2D u_BaseTexture;

void main()
{
    vec2 scrolledTexCoords = UV + u_Tile;
    FragColor = texture(u_BaseTexture, scrolledTexCoords) * vec4(u_Color, 1.0);
}