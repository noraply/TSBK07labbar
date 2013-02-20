#version 150
uniform mat4 projectionMatrix;
uniform mat4 mdlMatrix;
uniform mat4 CameraMatrix;
uniform float tid;

uniform sampler2D texunit;

in vec3 in_Position;
in vec3 in_Normal;
in vec2 in_TexCoord;


out vec3 ex_Normal;
out vec4 ex_colour;

void main(void)
{
	vec3 transform;
	mat4 myMatrix = projectionMatrix*CameraMatrix*mdlMatrix; 
	mat3 normMatrix = mat3(myMatrix);
	transform[0] = in_Position[0];
	transform[1] = in_Position[1];
	transform[2] = in_Position[2];	
	vec3 transNormal = normMatrix*in_Normal;
	gl_Position = myMatrix*vec4(transform, 1.0);
	ex_Normal = transNormal;
	ex_colour = texture(texunit, in_TexCoord);
}
