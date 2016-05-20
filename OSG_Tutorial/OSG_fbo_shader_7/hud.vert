
varying vec4 position;
void main()
{
	position = gl_ModelViewMatrix * gl_Vertex;
	gl_FrontColor = gl_Color;
	gl_Position = ftransform();
}