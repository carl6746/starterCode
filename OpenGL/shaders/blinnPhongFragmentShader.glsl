#version 330 core

layout(location=0) out vec4 fragmentColor;

uniform vec3 diffuseComponent;
uniform vec4 cameraPosWorld;

in vec4 normal;
in vec4 lightDir;
in vec4 vertexWorldPos;

void main(void)
{
  float val = max(0.0, dot(normal, lightDir));
  vec4 viewDir = vec4(normalize(cameraPosWorld-vertexWorldPos).xyz,0);
  vec4 hVector = normalize(lightDir + viewDir);
  float phongValue = pow(max(0.0, dot(normal,hVector)),64);
  


  vec3 diffuseShading = vec3( diffuseComponent.r * val, diffuseComponent.g * val, diffuseComponent.b * val ) + vec3(vec3(1,1,1).r*phongValue, vec3(1,1,1).g*phongValue, vec3(1,1,1).b*phongValue );
  vec3 clampedShader = clamp(diffuseShading, 0.0, 1.0);


  fragmentColor = vec4(clampedShader,1.0);
}