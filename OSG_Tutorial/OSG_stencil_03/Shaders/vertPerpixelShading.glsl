#version 400 compatibility

varying vec3 _NormalE;

void main()
{
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
	_NormalE = normalize(gl_NormalMatrix * gl_Normal);
}