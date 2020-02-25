#version 330 core

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec4 vertexColour;
layout(location = 2) in vec2 vertexTexCoords;

in vec4 vertexColourOut;
out vec2 vertexTexCoordOut;


uniform vec3 colorWeights;
uniform mat4 model;
uniform mat4 viewMat;
uniform mat4 projectionMat;

void main()
{
	vertexTexCoordOut = vertexTexCoords;
	mat4 mvp = projectionMat * viewMat * model;
	gl_Position = mvp*vec4(vertexPosition, 1.0f);
	//color = vertexColourOut;
}