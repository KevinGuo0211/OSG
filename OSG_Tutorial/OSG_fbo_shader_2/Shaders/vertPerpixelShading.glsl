#version 400 compatibility

layout (location = 0) in vec4 _VertexPosition;
layout (location = 1) in vec3 _VertexNormal;

varying vec3 _NormalE;

void main()
{
	gl_Position = gl_ModelViewProjectionMatrix * _VertexPosition;
	_NormalE = normalize(gl_NormalMatrix * gl_Normal);
}