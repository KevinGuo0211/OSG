#version 400 core

uniform sampler2D uColorBuf;
//uniform vec4 uColor;

in vec2 _TexCoord;
out vec4 _FragColor;

void main()
{
	_FragColor = texture(uColorBuf, _TexCoord);
	//_FragColor = uColor;
}