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
#include <stdio.h>
#include <math.h>

// Globals
Model *m;

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
	program = loadShaders("lab2-1.vert", "lab2-1.frag");
	printError("init shader");
	
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

GLfloat t = 0;
GLfloat matrix1[] = { 	1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f };
	
GLfloat matrix2[] = { 	1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f };

GLfloat matrix3[] = { 	1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f };

GLfloat matrix4[] = { 	1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f };

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	t += 0.02;
	
	matrix1[0] = cosf(t/4);
	matrix1[1] = -sinf(t/4);
	matrix1[4] = sinf(t/4);
	matrix1[5] = cosf(t/4);
	
	matrix2[5] = cosf(t/2);
	matrix2[6] = -sinf(t/2);
	matrix2[9] = sinf(t/2);
	matrix2[10] = cosf(t/2);

	matrix3[0] = cosf(3*t/4);
	matrix3[2] = sinf(3*t/4);
	matrix3[8] = -sinf(3*t/4);
	matrix3[10] = cosf(3*t/4);

	matrix4[3] = 0.5*cosf(t/2);
	matrix4[7] = 0.5*cosf(5*t/4);
	matrix4[11] = 0.3*sinf(t/16);

	glUniformMatrix4fv(glGetUniformLocation(program, "matrix1"), 1, GL_TRUE, matrix1);
	glUniformMatrix4fv(glGetUniformLocation(program, "matrix2"), 1, GL_TRUE, matrix2);
	glUniformMatrix4fv(glGetUniformLocation(program, "matrix3"), 1, GL_TRUE, matrix3);
	glUniformMatrix4fv(glGetUniformLocation(program, "matrix4"), 1, GL_TRUE, matrix4);

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
