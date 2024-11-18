#version 330 core

in vec3 ourColor;
in vec3 ourPosition;
out vec4 FragColor;
uniform float colorOffset;

void main() 
{
    FragColor = vec4(vec3(ourPosition.x+colorOffset,ourPosition.y+colorOffset,ourPosition.z+colorOffset), 1.0f);
}