#version 330 core

in vec3 vColor;
layout(location=0) out vec4 out_FragmentColor;

void main(void)
{
    out_FragmentColor = vec4(vColor,1.0);
}