#version 330 core

// vec4 uniform

out vec3 color;
uniform vec3 colorWeights;

float rand(vec2 co){
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

void main()
{
  color = colorWeights;
}