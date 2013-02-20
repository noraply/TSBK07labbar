#version 150

uniform vec3 lightSourcesDirPosArr[4];
uniform vec3 lightSourcesColorArr[4];
uniform float specularExponent[4];
uniform bool isDirectional[4];

uniform sampler2D texUnit;
uniform int i;

in vec3 ex_Normal;
in vec3 ex_Surface_Camera;
in vec2 ex_TexCoord;

out vec4 out_Color;

void main(void)
{
	float shade[4];
	for(int j = 0; j < 3; j++){
		shade[j] = dot(normalize(ex_Normal) , lightSourcesDirPosArr[j]) ;
		clamp(shade[j], 0, 1);
	}		
	if( i == 0 ){
	      	// Texturer som inte bör påverkas av ljuskällorna
		out_Color = texture(texUnit, ex_TexCoord);
	} else {
	       	out_Color = vec4(ex_Normal, 1.0); 
		out_Color += vec4(shade[2]*lightSourcesColorArr[2], 1.0); 
	}
}
