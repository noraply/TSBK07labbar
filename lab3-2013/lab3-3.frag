#version 150

uniform sampler2D texUnit;
uniform int i;

in vec3 ex_Normal;
in vec2 ex_TexCoord;

out vec4 out_Color;

void main(void)
{
	if( i == 0 ){
		out_Color = texture(texUnit, ex_TexCoord);
	} else {
		out_Color = vec4(ex_Normal, 1.0);
	}
}
