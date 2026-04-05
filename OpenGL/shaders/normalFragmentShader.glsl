#version 330 core

layout(location=0) out vec4 fragmentColor;

uniform vec3 diffuseComponent;

in vec4 normal;

void main(void)
{
  fragmentColor = vec4(normal);
}