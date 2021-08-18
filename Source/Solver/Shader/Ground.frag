#version 330 core

uniform sampler2D groundTexture;

in vec2 thisUV;

out vec4 FragColor;

void main()
{
    FragColor = texture(groundTexture, thisUV);
}