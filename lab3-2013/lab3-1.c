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
Model *walls, *wings, *roof, *balcony;

GLfloat projectionMatrix[] = {	2.0f*NEAR/(RIGHT-LEFT), 0.0f, (RIGHT + LEFT)/(RIGHT-LEFT), 0.0f,
				0.0f, 2.0f*NEAR/(TOP-BOTTOM), (TOP+BOTTOM)/(TOP-BOTTOM), 0.0f,
				0.0f, 0.0f, -(FAR+NEAR)/(FAR-NEAR), -2.0f*FAR*NEAR/(FAR-NEAR),
				0.0f, 0.0f, -1.0f, 0.0f };

GLfloat rot[16], trans[16], total[16], cam[16], up[3];
GLfloat t = 0;
GLfloat i = 0;
Point3D cam_pos;
Point3D lookAt_pos;

  
// Data would normally be read from files


// vertex array object
unsigned int bunnyVertexArrayObjID;

GLuint program;

void init(void)
{
  walls = LoadModelPlus("windmill/windmill-walls.obj");
  wings = LoadModelPlus("windmill/blade.obj");
  roof = LoadModelPlus("windmill/windmill-roof.obj");
  balcony = LoadModelPlus("windmill/windmill-balcony.obj");
  if(walls == NULL || wings == NULL ){
    fprintf(stderr, "Failed to load object\n");
  }

  dumpInfo();

  // GL inits
  glClearColor(0.7,0.2,0.5,1.0);
  glEnable(GL_DEPTH_TEST);
	
  printError("GL inits");

  // Load and compile shader
  program = loadShaders("lab3-1.vert", "lab3-1.frag");
  printError("init shader");
	
  glUniformMatrix4fv(glGetUniformLocation(program, "projectionMatrix"), 1, GL_TRUE, projectionMatrix);

  printError("init arrays");
}

void OnTimer(int value)
{
  glutPostRedisplay();
  glutTimerFunc(20, &OnTimer, value);
}

  GLfloat wing_di = 0.0;
  GLfloat wing_x = 4.5 ;
  GLfloat wing_y = 1.0;
  GLfloat wing_z = -25.0;


void display(void)
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
  t += 0.02;

  i = 0;	

  //Camera placement
  cam_pos.x = 25*sin(0.1*t);
  cam_pos.y = 0;
  cam_pos.z = -25 + 25*cos(0.1*t);
  lookAt_pos.x = 0;
  lookAt_pos.y = 0;
  lookAt_pos.z = -25.0;
  up[0]= 0.0f;
  up[1]= 1.0f;
  up[2] = 0.0f;
  glUniform1f(glGetUniformLocation(program, "tid"), t);
  lookAt(&cam_pos, &lookAt_pos, up[0],up[1],up[2], cam);
  glUniformMatrix4fv(glGetUniformLocation(program, "CameraMatrix"), 1, GL_TRUE, cam); 	
	

  printError("pre display");

  // vingarna
  T(wing_x, wing_y, wing_z, trans);
  Ry(M_PI/12, total);
  Rz(0, total);
  Rx(t / 2, rot);

  Mult(rot, total, total);
  Mult(trans, total, total);
  glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total);
	

  DrawModel(wings, program,"in_Position","in_Normal", NULL);

  T(wing_x, wing_y, wing_z, trans);
  Ry(M_PI/12, total);
  Rz(0, total);
  Rx(M_PI/2 + t / 2, rot);

  Mult(rot, total, total);
  Mult(trans, total, total);
  glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total);
	

  DrawModel(wings, program,"in_Position","in_Normal", NULL);
  
  T(wing_x, wing_y, wing_z, trans);
  Ry(M_PI/12, total);
  Rx(M_PI +  t / 2, rot);

  Mult(rot, total, total);
  Mult(trans, total, total);
  glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total);
	

  DrawModel(wings, program,"in_Position","in_Normal", NULL);
  
  T(wing_x, wing_y, wing_z, trans);
  Ry(M_PI/12, total);
  Rx(3*M_PI/2 + t / 2, rot);

  Mult(rot, total, total);
  Mult(trans, total, total);
  glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total);
	

  DrawModel(wings, program,"in_Position","in_Normal", NULL);

  // Möllan
  T(0,-8,-25, trans);
  Ry(i, rot);
	
  Mult(trans, rot, total);
  glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total);
  DrawModel(walls, program,"in_Position","in_Normal", NULL);

  // Taket
  T(0.0, -8.0, -25, trans);
  Ry(0, rot);
	
  Mult(trans, rot, total);
  glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total);
  DrawModel(roof, program,"in_Position","in_Normal", NULL);

  // Balkongen
  T(0.0, -8.2, -25, trans);
  Ry(i, rot);
	
  Mult(trans, rot, total);
  glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total);
  DrawModel(balcony, program,"in_Position","in_Normal", NULL);

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
