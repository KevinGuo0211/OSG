
	uniform sampler2D _rttCowTex;
uniform sampler2D _rttCowLen;
uniform sampler2D _rttCepTex;
uniform sampler2D _rttCepLen;

void main()
{
	vec4 _cowColor = texture2D(_rttCowTex, gl_TexCoord[0].xy);
	float _cowLen  = texture2D(_rttCowLen, gl_TexCoord[0].xy).a;

	vec4 _cepColor = texture2D(_rttCepTex, gl_TexCoord[0].xy);
	float _cepLen  = texture2D(_rttCepLen, gl_TexCoord[0].xy).a;

	vec4 _color = vec4(0.7,0.2,0.1,1.0);

	float alpha = 0.5;
	//比较深度， 哪个深度小就在前显示， 如果两物体近， 有适当的误差.
	if(_cepLen < _cowLen )
	{
		_color = _cepColor;
	}else
	{
		_color = _cowColor;
	}
	//去除背景色， 默认深度为1.0;
	if(_cepLen > 0.9999 && _cowLen > 0.9999)
		_color.a = 0.0;

	gl_FragColor = _color;
}