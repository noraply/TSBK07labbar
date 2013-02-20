#version 150

uniform vec3 lightSourcesDirPosArr[4];
uniform vec3 lightSourcesColorArr[4];
uniform float specularExponent[4];
uniform bool isDirectional[4];

uniform mat4 CameraMatrix;
uniform sampler2D skyTex;
uniform sampler2D grassTex;
uniform int i;
uniform float tid;

in vec3 ex_Normal;
in vec3 ex_Surface_Camera;
in vec2 ex_TexCoord;
in vec3 outPosition;

out vec4 out_Color;

void main(void)
{
	vec4 diffuse = vec4(0.0);
	vec4 specular = vec4(0.0);
	float shade[4];
	float spec[4];
	vec3 r;
	vec4 sky = texture(skyTex, ex_TexCoord);
	vec4 grass = texture(grassTex, ex_TexCoord);
	for(int j = 0; j < 4; j++){
		/* Directional lighting */
		if(isDirectional[j]){
			/* Diffuse shading */
			shade[j] = dot(normalize(ex_Normal) , normalize(lightSourcesDirPosArr[j]));
			shade[j] = max(shade[j], 0);
			
			/* Specular shading
			* s is -lightSourceDirPosArr[j] */
			r = reflect(-lightSourcesDirPosArr[j], ex_Normal); 
			// 2*ex_Normal*dot(-lightSourcesDirPosArr[j],ex_Normal) + lightSourcesDirPosArr[j];
			
		/* Point source lighting */			
		}else{
			/* Vector from surface to light source */
			vec3 direction = outPosition - lightSourcesDirPosArr[j]; 
			
			/* Diffuse shading */
			shade[j] = dot(normalize(ex_Normal), normalize(-direction));
			shade[j] = clamp(shade[j], 0, 1);

			/* Specular shading */
			r = reflect(direction, ex_Normal);
		}
		/* Add the specular exponent to the cosine function */
		spec[j] = pow(dot(normalize(r), normalize(ex_Surface_Camera)), specularExponent[j]);
		spec[j] = clamp(spec[j], 0, 1);
	}		
	out_Color = vec4(0.5, 0.5, 0.5, 1.0);	
	if( i == 0){
	      	// Texturer som inte bör påverkas av ljuskällorna
		out_Color = sky;
	}else if( i == 1){
	      out_Color = grass;
	}else if( i == 2 ){
	      out_Color = sin(0.3*tid)*sin(0.3*tid)*grass + cos(0.3*tid)*cos(0.3*tid)*sky;
	}
	
	/* Apply lightign to the object */
	if(i > 0){ 
		for(int j = 0; j < 4; j++){
			diffuse += out_Color*vec4(shade[j]*lightSourcesColorArr[j], 1.0);
			specular += out_Color*vec4(spec[j]*lightSourcesColorArr[j], 1.0);
		} 	
		out_Color += diffuse + specular;
	}
}
