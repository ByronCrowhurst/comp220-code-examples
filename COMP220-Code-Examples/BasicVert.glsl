#version 330 core
layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec4 vertexColour;

uniform mat4 model;
uniform mat4 viewMat;
uniform mat4 projectionMat;

out vec4 vertexColourOut;

void main()
{
	mat4 mvp = projectionMat * viewMat * model;
	vertexColourOut = vertexColour;
	gl_Position = mvp*vec4(vertexPosition, 1.0f);
}