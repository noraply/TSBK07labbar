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
Point3D obj_pos;
Point3D cam_pos;
GLfloat rot[16], trans[16], total[16], la[16], up[3];
GLfloat t = 0;

// Data would normally be read from files


// vertex array object
unsigned int bunnyVertexArrayObjID;

GLuint program;

void init(void)
{
	m = LoadModel("bunnyplus.obj");
	if(m == NULL){
		fprintf(stderr, "Failed to load object\n");
	}


	GLuint tex;
	LoadTGATextureSimple("maskros512.tga", &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	// two vertex buffer objects, used for uploading the

	unsigned int bunnyVertexBufferObjID;
	unsigned int bunnyIndexBufferObjID;
	unsigned int bunnyNormalBufferObjID;
	unsigned int bunnyTexCoordBufferObjID;

	dumpInfo();

	// GL inits
	glClearColor(0.7,0.2,0.5,1.0);
	glEnable(GL_DEPTH_TEST);
	
	printError("GL inits");

	// Load and compile shader
	program = loadShaders("lab2-6.vert", "lab2-6.frag");
	printError("init shader");
	
	glUniformMatrix4fv(glGetUniformLocation(program, "projectionMatrix"), 1, GL_TRUE, projectionMatrix);
	glUniform1i(glGetUniformLocation(program, "texUnit"), 0); // Texture unit 0
	// Upload geometry to the GPU:
	glGenVertexArrays(1, &bunnyVertexArrayObjID);
	glGenBuffers(1, &bunnyVertexBufferObjID);	
	glGenBuffers(1, &bunnyIndexBufferObjID);	
	glGenBuffers(1, &bunnyNormalBufferObjID);	
	glGenBuffers(1, &bunnyTexCoordBufferObjID);	
	
	glBindVertexArray(bunnyVertexArrayObjID);

	// Vertex VBO
	glBindBuffer(GL_ARRAY_BUFFER, bunnyVertexBufferObjID);
	glBufferData(GL_ARRAY_BUFFER, m->numVertices*3*sizeof(GLfloat), m->vertexArray, GL_STATIC_DRAW);
	glVertexAttribPointer(glGetAttribLocation(program, "in_Position"), 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(glGetAttribLocation(program, "in_Position"));

	// Normal VBO
	glBindBuffer(GL_ARRAY_BUFFER, bunnyNormalBufferObjID);
	glBufferData(GL_ARRAY_BUFFER, m->numVertices*3*sizeof(GLfloat), m->normalArray, GL_STATIC_DRAW);
	glVertexAttribPointer(glGetAttribLocation(program, "in_Normal"), 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(glGetAttribLocation(program, "in_Normal"));

	// Upload textures
	if(m->texCoordArray != NULL){
		glBindBuffer(GL_ARRAY_BUFFER, bunnyTexCoordBufferObjID);
		glBufferData(GL_ARRAY_BUFFER, m->numVertices*2*sizeof(GLfloat), m->texCoordArray, GL_STATIC_DRAW);
		glVertexAttribPointer(glGetAttribLocation(program, "in_TexCoord"), 2, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(glGetAttribLocation(program, "in_TexCoord"));
	}
	
	// Upload geomerty to GPU
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bunnyIndexBufferObjID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m->numIndices*sizeof(GLuint), m->indexArray, GL_STATIC_DRAW);


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

	// Camera
	obj_pos.x = sinf(t);
	obj_pos.y = 0.3*sinf(t);
	obj_pos.z = -3 + cosf(t);
	
	cam_pos.x = 0;
	cam_pos.y = 2;
	cam_pos.z = 1;
	
	up[0] = 0;
	up[1] = 1;
	up[2] = 0;
	
	
	
	lookAt(&cam_pos, &obj_pos, up[0], up[1], up[3], la);


	glUniformMatrix4fv(glGetUniformLocation(program, "cameraMatrix"), 1, GL_TRUE, la);

	T(sinf(t),0.3*sin(t),-3 + cosf(t),trans);
	Ry(t, rot);
	
	Mult(trans, rot, total);
	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total);
	
	printError("pre display");


	glBindVertexArray(bunnyVertexArrayObjID);	// Select VAO
	glDrawElements(GL_TRIANGLES, m->numIndices, GL_UNSIGNED_INT, 0L);	// draw object
	

	printError("display");

	glutSwapBuffers();
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutCreateWindow ("Laboration 3! Bunny version 2");
	glutDisplayFunc(display); 
	glutTimerFunc(20, &OnTimer, 0);
	init ();
	glutMainLoop();
}
