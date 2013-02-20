#version 150
uniform mat4 projectionMatrix;

uniform mat4 mdlMatrix;
uniform mat4 CameraMatrix;
uniform vec2 mouse_coord;
uniform int i;
uniform vec3 camera_pos;

in vec3 in_Position;
in vec3 in_Normal;
in vec2 in_TexCoord;


out vec3 ex_Normal;
out vec3 ex_colour;
out vec2 ex_TexCoord;
out vec3 ex_Surface_Camera;
out vec3 outPosition;

void main(void)
{

	ex_TexCoord = in_TexCoord;
	mat4 skyCam = CameraMatrix;
	mat4 myMatrix = projectionMatrix*CameraMatrix*mdlMatrix; 
	mat3 normMatrix = mat3(myMatrix);
	ex_Normal = mat3(mdlMatrix)*in_Normal;
	outPosition = vec3(mdlMatrix*vec4(in_Position, 1.0));
	
	/*-- If i=0, then we will draw the skyBox which should not be
        affected by movement of the camera*/
        if(i == 0){
	     skyCam[3][0] = 0;
	     skyCam[3][1] = 0;
	     skyCam[3][2] = 0;
	     gl_Position = projectionMatrix*skyCam*mdlMatrix*vec4(in_Position, 1.0);
	}else{
	    gl_Position = myMatrix*vec4(in_Position,1.0);

	}
	
	/* Vector from vertex to camera, will be interpolated between vertices */
	ex_Surface_Camera = camera_pos - outPosition;
}
