// simple vertex shader
varying vec4 pos;

void main()
{
	gl_Position    = gl_ModelViewProjectionMatrix * gl_Vertex;
	
	pos = gl_Vertex;
	//gl_FrontColor  = gl_Color;
	//gl_TexCoord[0] = gl_MultiTexCoord0;
}