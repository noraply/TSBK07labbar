#version 150

in vec3 ex_Normal;
//in vec2 ex_TexCoord;

in vec4 ex_colour;

out vec4 out_Color;

void main(void)
{
//	float a = sin(ex_TexCoord.s*20);
//	float b = cos(ex_TexCoord.s*20);
	out_Color = vec4(ex_colour);
}
