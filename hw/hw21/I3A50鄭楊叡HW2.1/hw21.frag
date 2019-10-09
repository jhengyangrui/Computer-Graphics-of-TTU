// simple fragment shader
varying vec4 pos;
// 'time' contains seconds since the program was linked.
uniform float time;

void main()
{
	vec4 color;
	
	if (pos.x > 0.0)
		color = vec4(1., 0., 1., 1.);
	else
		color = vec4(1., 1., 0., 1.);
	
	gl_FragColor = color;
}