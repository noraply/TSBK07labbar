#version 150
uniform mat4 projectionMatrix;

uniform mat4 mdlMatrix;
uniform mat4 CameraMatrix;
uniform vec2 mouse_coord;

in vec3 in_Position;
in vec3 in_Normal;
in vec2 in_TexCoord;
in vec3 camera_pos;


out vec3 ex_Normal;
out vec3 ex_colour;
out vec2 ex_TexCoord;
out vec3 ex_Surface_Camera;

void main(void)
{

	ex_TexCoord = in_TexCoord;
	mat4 myMatrix = projectionMatrix*CameraMatrix*mdlMatrix; 
	mat3 normMatrix = mat3(myMatrix);
	ex_Normal = normMatrix*in_Normal;
	gl_Position = myMatrix*vec4(in_Position, 1.0);
	
	/* Vector from vertex to camera, will be interpolated between vertices */
	ex_Surface_Camera[0] = camera_pos[0] - in_Position[0];
	ex_Surface_Camera[1] = camera_pos[1] - in_Position[1];
	ex_Surface_Camera[2] = camera_pos[2] - in_Position[2];

}
