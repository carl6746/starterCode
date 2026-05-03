#version 330 core

layout(location=0) out vec4 fragmentColor;

uniform vec4 cameraPosWorld;
uniform vec3 diffuseComponent;
uniform int numLights;

in vec4 normal;
in vec4 lightDir[8];
in vec4 vertexWorldPos;

void main(void)
{
    vec4 viewDir = vec4(normalize(cameraPosWorld-vertexWorldPos).xyz,0);
    vec3 color = vec3(0.0);


    for(int i = 0 ; i < numLights; ++i) {
        vec4 L = normalize(lightDir[i]);
        float val = max(0.0, dot(normal, L));

        vec4 hVector = normalize(L + viewDir);
        float phongValue = pow(max(0.0, dot(normal,hVector)),64);

        color += diffuseComponent * val;
        color += diffuseComponent * phongValue;
    }

    vec3 clampedShader = clamp(color, 0.0, 1.0);


    //fragmentColor = vec4(clampedShader,1.0);
    fragmentColor = vec4(normal.xyz * 0.5 + .5, 1.0);
}