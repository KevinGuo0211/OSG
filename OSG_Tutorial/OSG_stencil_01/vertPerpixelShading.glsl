#version 400 core

uniform mat4 osg_ModelViewProjectionMatrix;
uniform mat3 osg_NormalMatrix;

layout (location = 0) in vec4 _VertexPosition;
layout (location = 1) in vec3 _VertexNormal;

out vec3 _NormalE;

void main()
{
	gl_Position = osg_ModelViewProjectionMatrix * _VertexPosition;
	_NormalE = normalize(osg_NormalMatrix * _VertexNormal);
}