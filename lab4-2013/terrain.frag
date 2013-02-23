#version 150

out vec4 outColor;
in vec2 texCoord;
in vec3 ex_Normal;
uniform sampler2D tex;

void main(void)
{
	outColor = texture(tex, texCoord);
	outColor = vec4(abs(ex_Normal.x), abs(ex_Normal.y), abs(ex_Normal.z), 1.0);
}
