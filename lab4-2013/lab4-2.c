// Lab 4, terrain generation

#ifdef __APPLE__
	#include <OpenGL/gl3.h>
	#include "MicroGlut.h"
	// Linking hint for Lightweight IDE
	// uses framework Cocoa
#endif
#include "GL_utilities.h"
#include "VectorUtils2.h"
#include "loadobj.h"
#include "LoadTGA2.h"

GLfloat projectionMatrix[16];

GLfloat rot[16], trans[16], total[16], camMatrix[16], cam_mod[16];
GLfloat t = 0;
GLfloat i = 0;
Point3D cam = {0, 5, 8};
Point3D lookAtPoint = {2, 0, 2};
Point3D up;


Model* GenerateTerrain(TextureData *tex)
{
	int vertexCount = tex->width * tex->height;
	int triangleCount = (tex->width-1) * (tex->height-1) * 2;
	int x, z;
	
	GLfloat *vertexArray = malloc(sizeof(GLfloat) * 3 * vertexCount);
	GLfloat *normalArray = malloc(sizeof(GLfloat) * 3 * vertexCount);
	GLfloat *texCoordArray = malloc(sizeof(GLfloat) * 2 * vertexCount);
	GLuint *indexArray = malloc(sizeof(GLuint) * triangleCount*3);
	
	printf("bpp %d\n", tex->bpp);
	for (x = 0; x < tex->width; x++)
		for (z = 0; z < tex->height; z++)
		{
// Vertex array. You need to scale this properly
			vertexArray[(x + z * tex->width)*3 + 0] = x / 1.0;
			vertexArray[(x + z * tex->width)*3 + 1] = tex->imageData[(x + z * tex->width) * (tex->bpp/8)] / 10.0;
			vertexArray[(x + z * tex->width)*3 + 2] = z / 1.0;
// Normal vectors. You need to calculate these.
			normalArray[(x + z * tex->width)*3 + 0] = 0.0;
			normalArray[(x + z * tex->width)*3 + 1] = 1.0;
			normalArray[(x + z * tex->width)*3 + 2] = 0.0;
// Texture coordinates. You may want to scale them.
			texCoordArray[(x + z * tex->width)*2 + 0] = x; // (float)x / tex->width;
			texCoordArray[(x + z * tex->width)*2 + 1] = z; // (float)z / tex->height;
		}
	for (x = 0; x < tex->width-1; x++)
		for (z = 0; z < tex->height-1; z++)
		{
		// Triangle 1
			indexArray[(x + z * (tex->width-1))*6 + 0] = x + z * tex->width;
			indexArray[(x + z * (tex->width-1))*6 + 1] = x + (z+1) * tex->width;
			indexArray[(x + z * (tex->width-1))*6 + 2] = x+1 + z * tex->width;
		// Triangle 2
			indexArray[(x + z * (tex->width-1))*6 + 3] = x+1 + z * tex->width;
			indexArray[(x + z * (tex->width-1))*6 + 4] = x + (z+1) * tex->width;
			indexArray[(x + z * (tex->width-1))*6 + 5] = x+1 + (z+1) * tex->width;
		}
	
	// End of terrain generation
	
	// Create Model and upload to GPU:

	Model* model = LoadDataToModel(
			vertexArray,
			normalArray,
			texCoordArray,
			NULL,
			indexArray,
			vertexCount,
			triangleCount*3);

	return model;
}


// vertex array object
Model *m, *m2, *tm;
// Reference to shader program
GLuint program;
GLuint tex1, tex2;
TextureData ttex; // terrain

void init(void)
{
	// GL inits
	glClearColor(0.2,0.2,0.5,0);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	printError("GL inits");

	frustum(-0.1, 0.1, -0.1, 0.1, 0.2, 50.0, projectionMatrix);

	// Load and compile shader
	program = loadShaders("terrain.vert", "terrain.frag");
	glUseProgram(program);
	printError("init shader");
	
	glUniformMatrix4fv(glGetUniformLocation(program, "projMatrix"), 1, GL_TRUE, projectionMatrix);
	glUniform1i(glGetUniformLocation(program, "tex"), 0); // Texture unit 0
	LoadTGATextureSimple("maskros512.tga", &tex1);
	
// Load terrain data
	
	LoadTGATexture("fft-terrain.tga", &ttex);
	tm = GenerateTerrain(&ttex);
	printError("init terrain");
}

void check_keyboard()
{
  Point3D r;
  Point3D right;
  r.x = lookAtPoint.x - cam.x; 
  r.y = lookAtPoint.y - cam.y;
  r.z = lookAtPoint.z - cam.z;
  
  up.x = 0;
  up.y = 1;
  up.z = 0;
  Normalize(&r);


  CrossProduct(&r, &up, &right);
  CrossProduct(&right, &r, &up);
  if(keyIsDown('q')){
    exit(0);
  } else if(keyIsDown('w')){
    cam.x += r.x;
    cam.y += r.y;
    cam.z += r.z;
    lookAtPoint.x += r.x;
    lookAtPoint.y += r.y;
    lookAtPoint.z += r.z;
    
  } else if(keyIsDown('s')){
    cam.x -= r.x;
    cam.y -= r.y;
    cam.z -= r.z;

    lookAtPoint.x -= r.x;
    lookAtPoint.y -= r.y;
    lookAtPoint.z -= r.z;

  } else if(keyIsDown('a')){
    cam.x -= right.x;
    cam.z -= right.z;
    
    lookAtPoint.x -= right.x;
    lookAtPoint.z -= right.z;
  } else if(keyIsDown('d')){
    cam.x += right.x;
    cam.z += right.z;

    lookAtPoint.x += right.x;
    lookAtPoint.z += right.z;
  } else if(keyIsDown('u')){
    cam.x = 0;
    cam.y = 5;
    cam.z = 8;
    lookAtPoint.z = 2;
    lookAtPoint.x = 0;
    lookAtPoint.y = 2;
  } else if(keyIsDown('j')){
    lookAtPoint.x -= 2*right.x;
    lookAtPoint.z -= 2*right.z;
  } else if(keyIsDown('l')){
    lookAtPoint.x += 2*right.x;
    lookAtPoint.z += 2*right.z;
  } else if(keyIsDown('i')){
    lookAtPoint.y +=  up.y*0.5;
  } else if(keyIsDown('k')){
    lookAtPoint.y -= up.y*0.5;
  }
}
void display(void)
{

	check_keyboard();
	// clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	GLfloat total[16], modelView[16];
	
	printError("pre display");
	
	glUseProgram(program);

	// Build matrix
	
	lookAt(&cam,
				&lookAtPoint,
				0, 1, 0,
				camMatrix);
	IdentityMatrix(modelView);
	Mult(camMatrix, modelView, total);
	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total);
	
	glBindTexture(GL_TEXTURE_2D, tex1);		// Bind Our Texture tex1
	DrawModel(tm, program, "inPosition", "inNormal", "inTexCoord");

	printError("display 2");
	
	glutSwapBuffers();
}

void timer(int i)
{
	glutTimerFunc(20, &timer, i);
	glutPostRedisplay();
}

void mouse(int x, int y)
{
	GLfloat phi;
	GLfloat theta;
	phi = (float)(x)/glutGet(GLUT_WINDOW_WIDTH)*2*M_PI;
	theta = (float)(y)/glutGet(GLUT_WINDOW_HEIGHT)*2*M_PI;

	lookAtPoint.x = cam.x - sin(theta)*sin(phi);
	lookAtPoint.y = cam.y + cos(theta);
	lookAtPoint.z = cam.z + sin(theta)*cos(phi);
}



int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize (600, 600);
	glutCreateWindow ("TSBK07 Lab 4");
	glutDisplayFunc(display);
	init ();
	initKeymapManager();
	glutTimerFunc(20, &timer, 0);

	glutPassiveMotionFunc(mouse);

	glutMainLoop();
	exit(0);
}
