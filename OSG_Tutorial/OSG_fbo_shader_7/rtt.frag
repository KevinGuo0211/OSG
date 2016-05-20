



uniform float zFar;  
uniform float zNear;  
varying vec4 position;  

void main()  
{  
	float zDiff = zFar - zNear;  
	float _len = length(position); 
	float interpolatedDepth = _len / (zFar - zNear);

	gl_FragData[0] = gl_Color;//vec4(1.0,0.0,0.0,1.0);
	gl_FragData[1] = vec4(vec3(0.0),interpolatedDepth);

}