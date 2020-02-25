#version 330 core

in vec2 vertexTexCoordOut;
out vec4 color;
uniform vec3 colorWeights;
uniform sampler2D baseTexture;

void main()
{
	color = texture(baseTexture, vertexTexCoordOut);
}