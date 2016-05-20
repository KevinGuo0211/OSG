#version 400 core

uniform sampler2D uColorBuf0;
uniform sampler2D uColorBuf1;
//uniform vec4 uColor;

in vec2 _TexCoord;
out vec4 _FragColor;

void main()
{
	vec4 FragColor0 = texture(uColorBuf0, _TexCoord);
	vec4 FragColor1 = texture(uColorBuf1, _TexCoord);

	if (gl_FragCoord.x > 512)
		_FragColor = FragColor0;
	else
		_FragColor = FragColor1;
}