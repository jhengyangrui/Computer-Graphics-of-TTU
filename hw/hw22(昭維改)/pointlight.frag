// simple fragment shader
varying vec4 eyepos;
varying vec3 eyenorm;
uniform sampler2D tex;
uniform sampler2D tex1;
varying vec4 objpos;
void main()
{
    vec3 light = gl_LightSource[0].position.xyz - 
		eyepos.xyz/eyepos.w;
	float ndotl = dot (normalize(eyenorm), normalize(light)); 

	// half = eye + l
	vec3 halfV = -normalize(eyepos.xyz/eyepos.w) + normalize(light);
	float ndoth = dot (normalize (eyenorm),normalize (halfV)); 	
	
	vec4 colorsum = gl_FrontLightProduct[0].ambient;
	colorsum += ndotl * gl_FrontLightProduct[0].diffuse;
	colorsum += pow (ndoth , gl_FrontMaterial.shininess)
       * gl_FrontLightProduct[0].specular;

	gl_FragColor = colorsum + texture2D (tex, objpos.xy) * texture2D (tex1, objpos.xy);//colorsum;
}
