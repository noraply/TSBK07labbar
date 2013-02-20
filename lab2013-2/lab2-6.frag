#version 150

uniform sampler2D texunit;

in vec2 ex_TexCoord;
in float shade;

out vec4 out_Color;

void main(void)
{
	out_Color = shade*texture(texunit, ex_TexCoord);
}
