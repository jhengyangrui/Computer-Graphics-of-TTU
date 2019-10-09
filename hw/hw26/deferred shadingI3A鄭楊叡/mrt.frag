// MRT [0]: diffuse
// MRT [1]: normal
// MRT [2]: depth

varying vec3 eyenormal;
varying vec4 eyepos;

void main ()
{
	//vec3 light = gl_LightSource[0].position.xyz - 
		eyepos.xyz/eyepos.w;
	//float ndotl = dot (normalize(eyenorm), normalize(light)); 
	
	
	vec3 halfv = gl_LightSource[0].halfVector.xyz;
	float ndoth = dot(normalize(eyenormal),normalize(halfv));	
	
// normal treatment ...
	vec3 frag_n = normalize (eyenormal); // [-1,1]^3
	frag_n = (frag_n + vec3 (1.))/2.; // [0,1]^3	

	// depth treatment ...
	vec4 clipos = gl_ProjectionMatrix * eyepos;
	float depth = clipos.z/clipos.w;
	depth = (depth + 1.0)/2.0;
	
	
	
	//gl_FragData[0] = colorsum;
	gl_FragData[0] = gl_FrontMaterial.diffuse+pow (ndoth , gl_FrontMaterial.shininess)
       * gl_FrontLightProduct[0].specular; 
	gl_FragData[1] = vec4 (frag_n, 1.0);
	gl_FragData[2] = vec4 (vec3(depth),1.0);
	
}
/*
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
}*/