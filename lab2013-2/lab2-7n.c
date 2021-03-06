// Lab 1-1.
// This is the same as the first simple example in the course book,
// but with a few error checks.
// Remember to copy your file to a new on appropriate places during the lab so you keep old results.
// Note that the files "lab1-1.frag", "lab1-1.vert" are required.

// Includes vary a bit with platforms.
// MS Windows needs GLEW or glee.
// Mac uses slightly different paths.

#include "GL_utilities.h"
#include "loadobj.h"
#include "VectorUtils2.h"
#include "LoadTGA.h"
#include <stdio.h>
#include <math.h>

#define NEAR 1.0
#define FAR 30.0
#define RIGHT 0.5
#define LEFT -0.5
#define TOP 0.5
#define BOTTOM -0.5

// Globals
Model *m;

GLfloat projectionMatrix[] = {	2.0f*NEAR/(RIGHT-LEFT), 0.0f, (RIGHT + LEFT)/(RIGHT-LEFT), 0.0f,
				0.0f, 2.0f*NEAR/(TOP-BOTTOM), (TOP+BOTTOM)/(TOP-BOTTOM), 0.0f,
				0.0f, 0.0f, -(FAR+NEAR)/(FAR-NEAR), -2.0f*FAR*NEAR/(FAR-NEAR),
				0.0f, 0.0f, -1.0f, 0.0f };

GLfloat rot[16], trans[16], total[16], cam[16], up[3];
GLfloat t = 0;
Point3D cam_pos;
Point3D lookAt_pos;

// Data would normally be read from files


// vertex array object
unsigned int bunnyVertexArrayObjID;

GLuint program;

void init(void)
{
  m = LoadModelPlus("bunnyplus.obj");
  if(m == NULL){
    fprintf(stderr, "Failed to load object\n");
  }


  GLuint tex;
  LoadTGATextureSimple("maskros512.tga", &tex);
  glBindTexture(GL_TEXTURE_2D, tex);
	
  dumpInfo();

  // GL inits
  glClearColor(0.7,0.2,0.5,1.0);
  glEnable(GL_DEPTH_TEST);
	
  printError("GL inits");

  // Load and compile shader
  program = loadShaders("lab2-7n.vert", "lab2-7n.frag");
  printError("init shader");
	
  glUniformMatrix4fv(glGetUniformLocation(program, "projectionMatrix"), 1, GL_TRUE, projectionMatrix);
  glUniform1i(glGetUniformLocation(program, "texUnit"), 0); // Texture unit 0

  printError("init arrays");
}

void OnTimer(int value)
{
  glutPostRedisplay();
  glutTimerFunc(20, &OnTimer, value);
}



void display(void)
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
  t += 0.02;
	
  //Camera placement
  cam_pos.x = 3*sin(t);
  cam_pos.y = 6;
  cam_pos.z = -4 + 3*cos(t);
  lookAt_pos.x = 0;
  lookAt_pos.y = 0;
  lookAt_pos.z = -4.0;
  up[0]= 1.0f;
  up[1]= 0.0f;
  up[2] = 0.0f;
  glUniform1f(glGetUniformLocation(program, "tid"), t);
  lookAt(&cam_pos, &lookAt_pos, up[0],up[1],up[2], cam);
  glUniformMatrix4fv(glGetUniformLocation(program, "CameraMatrix"), 1, GL_TRUE, cam); 	


  T(1.0,0,-3,trans);
  Ry(0, rot);
	
  Mult(trans, rot, total);
  glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total);
	
  printError("pre display");

  DrawModel(m, program,"in_Position","in_Normal","in_TexCoord");
  
  T(0,0,-5, trans);
  Ry(0, rot);
	
  Mult(trans, rot, total);
  glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total);
  DrawModel(m, program,"in_Position","in_Normal","in_TexCoord");

  printError("display");
  glutSwapBuffers();
}

int main(int argc, char *argv[])
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
  glutCreateWindow ("Laboration 2! Double bunny");
  glutDisplayFunc(display); 
  glutTimerFunc(20, &OnTimer, 0);
  init ();
  glutMainLoop();
}
