#version 330 core

layout(location=0) out vec4 fragmentColor;

uniform vec3 diffuseComponent;

in vec4 normal;
in vec4 lightDir;
in vec3 color;

void main(void)
{
  float val = max(0.0, dot(normal, lightDir));
  vec3 diffuseShading = vec3( color.r * val, color.g * val, color.b * val );

  fragmentColor = vec4(diffuseShading, 1.0);
}