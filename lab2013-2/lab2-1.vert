#version 150

uniform mat4 matrix1;
uniform mat4 matrix2;
uniform mat4 matrix3;
uniform mat4 matrix4;

in  vec3 in_Position;
in vec3 in_Normal;
in vec2 in_TexCoord;


out vec3 ex_Normal;
out vec2 ex_TexCoord;

void main(void)
{
	mat4 myMatrix = matrix4*matrix3*matrix2*matrix1;
	mat3 normMatrix = mat3(myMatrix);

	vec3 transNormal = normMatrix*in_Normal;
	gl_Position = myMatrix*vec4(in_Position, 1.0);
	ex_Normal = transNormal;
	ex_TexCoord = in_TexCoord;
}
