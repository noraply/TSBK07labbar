#version 150
uniform mat4 projectionMatrix;
uniform mat4 mdlMatrix;
uniform mat4 CameraMatrix;
uniform vec2 mouse_coord;

in vec3 in_Position;
in vec3 in_Normal;
in vec2 in_TexCoord;


out vec3 ex_Normal;
out vec3 ex_colour;
out vec2 ex_TexCoord;

void main(void)
{
	ex_TexCoord = in_TexCoord;
	vec3 transform;
	mat4 myMatrix = projectionMatrix*CameraMatrix*mdlMatrix; 
	mat3 normMatrix = mat3(myMatrix);
	transform[0] = in_Position[0];
	transform[1] = in_Position[1];
	transform[2] = in_Position[2];	
	ex_Normal = normMatrix*in_Normal;
	gl_Position = myMatrix*vec4(transform, 1.0);
}
