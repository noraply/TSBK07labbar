#version 150
uniform mat4 projectionMatrix;
uniform mat4 mdlMatrix;

uniform sampler2D texunit;

in vec3 in_Position;
in vec3 in_Normal;
in vec2 in_TexCoord;


out vec3 ex_Normal;
out vec4 ex_colour;

void main(void)
{
	mat4 myMatrix = projectionMatrix*mdlMatrix; 
	mat3 normMatrix = mat3(myMatrix);

	vec3 transNormal = normMatrix*in_Normal;
	gl_Position = myMatrix*vec4(in_Position, 1.0);
	ex_Normal = transNormal;
	ex_colour = texture(texunit, in_TexCoord);
}
