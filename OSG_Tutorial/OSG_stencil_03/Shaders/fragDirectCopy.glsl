#version 400 core

uniform sampler2D uColorBuf;

in vec2 _TexCoord;
out vec4 _FragColor;

void main()
{
	_FragColor = texture(uColorBuf, _TexCoord);
}