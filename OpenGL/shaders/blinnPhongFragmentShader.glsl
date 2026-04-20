#version 330 core

layout(location=0) out vec4 fragmentColor;

uniform vec4 cameraPosWorld;
uniform vec3 diffuseComponent;

in vec4 normal;
in vec4 lightDir;
in vec4 vertexWorldPos;
in vec3 color;

void main(void)
{
  float val = max(0.0, dot(normal, lightDir));
  vec4 viewDir = vec4(normalize(cameraPosWorld-vertexWorldPos).xyz,0);
  vec4 hVector = normalize(lightDir + viewDir);
  float phongValue = pow(max(0.0, dot(normal,hVector)),64);
  //vec3 color = diffuseComponent;
  


  vec3 diffuseShading = vec3( color.r * val, color.g * val, color.b * val ) + vec3(color.r*phongValue, color.g*phongValue, color.b*phongValue );
  vec3 clampedShader = clamp(diffuseShading, 0.0, 1.0);


  fragmentColor = vec4(diffuseShading,1.0);
}