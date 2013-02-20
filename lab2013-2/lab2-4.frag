#version 150

uniform sampler2D texunit;

in vec3 ex_Normal;
in vec2 ex_TexCoord;

const vec3 light = vec3(0.58, 0.58, 0.58);


out vec4 out_Color;

void main(void)
{
	float shade = dot(normalize(ex_Normal), light);
	shade = clamp(0,1,shade);
	out_Color = shade*texture(texunit, ex_TexCoord);
}
