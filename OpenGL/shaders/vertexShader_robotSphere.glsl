#version 330 core

layout(location=0) in vec3 in_Position;
layout(location=1) in vec3 in_Normal;

uniform mat4 projMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;
uniform mat4 normalMatrix;

uniform vec4 lightPosWorld[8];
uniform int numLights;

out vec4 normal;
out vec4 lightDir[8];
out vec4 vertexWorldPos;


void main(void)
{
  normal = normalize(normalMatrix * vec4(in_Normal, 0.0));

  vertexWorldPos = modelMatrix * vec4(in_Position, 1.0);

  for(int i = 0; i < numLights; ++i) {
    vec3 L = normalize(lightPosWorld[i].xyz - vertexWorldPos.xyz);
    lightDir[i] = vec4(L,0.0);
  }

  gl_Position = projMatrix * viewMatrix * vertexWorldPos;
}