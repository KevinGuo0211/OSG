#version 400 core

uniform vec3 uLightDirectionE = vec3(1, 0, 0);
uniform vec3 uMaterialAmbient = vec3(0.3, 0.3, 0.3);
uniform vec3 uMaterialDiffuse = vec3(0.9, 0.9, 0.9);
uniform vec3 uLightAmbient = vec3(0.6, 0.6, 0.6);
uniform vec3 uLightDiffuse = vec3(0.9, 0.9, 0.9);

in vec3  _NormalE;
//out vec4 _FragColor;

void main()
{
	vec3 Ambient = uLightAmbient * uMaterialAmbient;
	gl_FragData[0] = vec4(Ambient, 1);
	
	vec3 Normal = normalize(_NormalE);
	float t = dot(normalize(uLightDirectionE), Normal);
	if (t < 0.0)
	{
		vec3 DiffuseColor = -uLightDiffuse * uMaterialDiffuse * t;
		gl_FragData[0].xyz += DiffuseColor;
	}
	gl_FragData[1] = vec4(1.0, 1.0, 0.0, 1.0);
}