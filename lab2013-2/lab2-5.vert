#version 150
uniform mat4 cameraMatrix;
uniform mat4 projectionMatrix;
uniform mat4 mdlMatrix;


in vec3 in_Position;
in vec3 in_Normal;
in vec2 in_TexCoord;


out vec3 ex_Normal;
out vec2 ex_TexCoord;

void main(void)
{
	mat4 myMatrix = projectionMatrix*cameraMatrix*mdlMatrix; 
	mat3 normMatrix = mat3(myMatrix);

	ex_Normal = normMatrix*in_Normal;
	gl_Position = myMatrix*vec4(in_Position, 1.0);
	ex_TexCoord = in_TexCoord;
}
