#version 330 core

layout(location = 0) in vec3 vertexPosition;

uniform mat4 model;
uniform mat4 viewMat;
uniform mat4 projectionMat;

void main()
{
	mat4 mvp = projectionMat * viewMat * model;
	gl_Position = mvp*vec4(vertexPosition, 1.0f);
}