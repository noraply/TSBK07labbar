#include "GL_utilities.h"
#include "loadobj.h"
#include "VectorUtils2.h"
#include "LoadTGA.h"
#include <stdio.h>
#include <math.h>

#define NEAR 1.0
#define FAR 500.0
#define RIGHT 0.5
#define LEFT -0.5
#define TOP 0.5
#define BOTTOM -0.5

// Globals
Model *walls, *wings, *roof, *balcony;
Model *skybox;
Model *bunny;

GLfloat projectionMatrix[] = {	2.0f*NEAR/(RIGHT-LEFT), 0.0f, (RIGHT + LEFT)/(RIGHT-LEFT), 0.0f,
				0.0f, 2.0f*NEAR/(TOP-BOTTOM), (TOP+BOTTOM)/(TOP-BOTTOM), 0.0f,
				0.0f, 0.0f, -(FAR+NEAR)/(FAR-NEAR), -2.0f*FAR*NEAR/(FAR-NEAR),
				0.0f, 0.0f, -1.0f, 0.0f };

GLfloat rot[16], trans[16], total[16], cam[16], cam_mod[16];
GLfloat t = 0;
GLfloat i = 0;
Point3D cam_pos;
Point3D lookAt_pos;
Point3D up;

// Ljus relaterat
Point3D lightSourcesColorsArr[] = { {1.0f, 0.0f, 0.0f}, // Red light
				    {0.0f, 1.0f, 0.0f}, // Green light
				    {0.0f, 0.0f, 1.0f}, // Blue light
				    {1.0f, 1.0f, 1.0f} }; // White light
Point3D lightSourcesDirectionsPositions[] = { {10.0f, 5.0f, 0.0f}, // Red light, positional
					      {0.0f, 5.0f, 10.0f}, // Green light, positional
					      {-1.0f, 0.0f, 0.0f}, // Blue light along X
					      {0.0f, 0.0f, -1.0f} }; // White light along Z
GLfloat specularExponent[] = {10.0, 20.0, 60.0, 5.0};
GLint isDirectional[] = {0,0,1,1};
//

GLfloat mouse_coord[2];
  
GLfloat ground[] = {	-FAR, BOTTOM, FAR,
			-FAR, BOTTOM,  -FAR,
			FAR, BOTTOM, -FAR,
			FAR, BOTTOM, -FAR,
			FAR, BOTTOM, FAR,
			-FAR, BOTTOM, FAR };
GLfloat gNormal[] = { 0.0f, 0.0f, 0.0f,
		      0.0f, 0.0f, 0.0f,
		      0.0f, 0.0f, 0.0f,
		      0.0f, 0.0f, 0.0f,
		      0.0f, 0.0f, 0.0f,
		      0.0f, 0.0f, 0.0f };

GLfloat ground_tex_coord[6][2] = {	{1.0, 0.0},
					{0.0, 0.0},
					{0.0, 1.0},
					{0.0, 1.0},
					{1.0, 1.0},
					{1.0, 0.0} };
// GLuint ground_indices[6] = {	0,1,3, 1,3,2 };
// 
GLuint ground_tex;
GLuint skybox_tex;
unsigned int groundVertexArrayObjID;
unsigned int gNormalArrayObjID;

GLuint program;

void init(void)
{
  glEnable(GL_TEXTURE_2D);
  walls = LoadModelPlus("windmill/windmill-walls.obj");
  wings = LoadModelPlus("windmill/blade.obj");
  roof = LoadModelPlus("windmill/windmill-roof.obj");
  balcony = LoadModelPlus("windmill/windmill-balcony.obj");
  skybox = LoadModelPlus("skybox.obj");
  bunny = LoadModelPlus("bunnyplus.obj");

  LoadTGATextureSimple("grass.tga", &ground_tex);
  LoadTGATextureSimple("SkyBox512.tga", &skybox_tex);
  

  glActiveTexture(GL_TEXTURE0); // Activate ground texture
  glBindTexture(GL_TEXTURE_2D, ground_tex); 

  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, skybox_tex);

  if(walls == NULL || wings == NULL ){
    fprintf(stderr, "Failed to load object\n");
  }
  cam_pos.x = 0.0;
  cam_pos.y = 0.0;
  cam_pos.z = 0.0;
  
  lookAt_pos.x = cam_pos.x;
  lookAt_pos.y = cam_pos.y;
  lookAt_pos.z = cam_pos.z - 1;
 
  up.x = 0;
  up.y = 1;
  up.z = 0;

  dumpInfo();

  // GL inits
  glClearColor(0.7,0.2,0.5,1.0);
  glEnable(GL_DEPTH_TEST);
	
  printError("GL inits");

  // Load and compile shader
  program = loadShaders("lab3-5.vert", "lab3-5.frag");
  printError("init shader");
	
  glUniformMatrix4fv(glGetUniformLocation(program, "projectionMatrix"), 1, GL_TRUE, projectionMatrix);

  // Laddar upp ljusk�llor
  glUniform3fv(glGetUniformLocation(program, "lightSourcesDirPosArr"), 4, &lightSourcesDirectionsPositions[0].x);
  glUniform3fv(glGetUniformLocation(program, "lightSourcesColorArr"), 4, &lightSourcesColorsArr[0].x);
  glUniform1fv(glGetUniformLocation(program, "specularExponent"), 4, specularExponent);
  glUniform1iv(glGetUniformLocation(program, "isDirectional"), 4, isDirectional); 
 
 
  printError("init arrays");
}

void check_keyboard()
{
  Point3D r;
  Point3D right;
  r.x = lookAt_pos.x - cam_pos.x; 
  r.y = lookAt_pos.y - cam_pos.y;
  r.z = lookAt_pos.z - cam_pos.z;
  
  up.x = 0;
  up.y = 1;
  up.z = 0;
  Normalize(&r);

  CrossProduct(&r, &up, &right);
  CrossProduct(&right, &r, &up);
  if(keyIsDown('q')){
    exit(0);
  } else if(keyIsDown('w')){
    cam_pos.x += r.x;
    cam_pos.z += r.z;
    lookAt_pos.x += r.x;
    lookAt_pos.z += r.z;
    
  } else if(keyIsDown('s')){
    cam_pos.x -= r.x;
    cam_pos.z -= r.z;
  } else if(keyIsDown('a')){
    cam_pos.x -= right.x;
    cam_pos.z -= right.z;
    
    lookAt_pos.x -= right.x;
    lookAt_pos.z -= right.z;
  } else if(keyIsDown('d')){
    cam_pos.x += right.x;
    cam_pos.z += right.z;

    lookAt_pos.x += right.x;
    lookAt_pos.z += right.z;
  } else if(keyIsDown('u')){
    cam_pos.x = 0;
    cam_pos.y = 0;
    cam_pos.z = 1;
    lookAt_pos.z = -1;
    lookAt_pos.x = 0;
    lookAt_pos.y = 0;
  } else if(keyIsDown('j')){
    lookAt_pos.x -= 2*right.x;
    lookAt_pos.z -= 2*right.z;
  } else if(keyIsDown('l')){
    lookAt_pos.x += 2*right.x;
    lookAt_pos.z += 2*right.z;
  } else if(keyIsDown('i')){
    lookAt_pos.y += 0.1* up.y;
  } else if(keyIsDown('k')){
    lookAt_pos.y -= 0.1*up.y;
  }
}

  void OnTimer(int value)
      {
	glutPostRedisplay();
	glutTimerFunc(20, &OnTimer, value);
      }

  void MouseMove(int x, int y)
  {
    int width = glutGet(GLUT_WINDOW_WIDTH);
    int height = glutGet(GLUT_WINDOW_HEIGHT);
    lookAt_pos.x +=(GLfloat) 2*(x-width/2)/width;
    lookAt_pos.y -= (GLfloat) 2*(y - height/2)/height;

  }

  GLfloat wing_di = 0.0;
  GLfloat wing_x = 4.5 ;
  GLfloat wing_y = 1.0;
  GLfloat wing_z = -25.0;


  void display(void)
  {
    check_keyboard();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
    t += 0.02;

    i = 0;	

    glUniform1i(glGetUniformLocation(program, "i"), i);
    
     
    //Camera placement
    glUniform1f(glGetUniformLocation(program, "tid"), t);
    lookAt(&cam_pos, &lookAt_pos, up.x,up.y,up.z, cam);

    IdentityMatrix(total);
    T(0.0, -0.2f, 0.0f, total);

    glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total);    
    glUniformMatrix4fv(glGetUniformLocation(program, "CameraMatrix"), 1, GL_TRUE, cam); 
    glUniform3f(glGetUniformLocation(program, "camera_pos"), cam_pos.x, cam_pos.y, cam_pos.z);
	

    printError("pre display");

    glDisable(GL_DEPTH_TEST);
    // glDisable(GL_CULL_FACE);
    glUniform1i(glGetUniformLocation(program, "skyTex"), 1); // Texture unit 1 
    DrawModel(skybox, program, "in_Position", "in_Normal", "in_TexCoord");
    //glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);

    glUniformMatrix4fv(glGetUniformLocation(program, "CameraMatrix"), 1, GL_TRUE, cam); 	  	    
    
    // Marken
    i = 1;
    glUniform1i(glGetUniformLocation(program, "i"), i);
    glUniform1i(glGetUniformLocation(program, "grassTex"), 0); // Texture unit 0
    
    T(0.0, -7.0, -.0f, total);
    glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total);

    

    unsigned int groundBufferObjID;
    unsigned int groundTexCoordBufferObjID;
    unsigned int gNormalBufferObjID;
    glGenVertexArrays(1, &groundVertexArrayObjID);
    glGenVertexArrays(1, &gNormalArrayObjID);
    glBindVertexArray(groundVertexArrayObjID);
    glGenBuffers(1, &groundBufferObjID);

    // Vertices
    glBindBuffer(GL_ARRAY_BUFFER, groundBufferObjID);
    glBufferData(GL_ARRAY_BUFFER, 6*3*sizeof(GLfloat), ground, GL_STATIC_DRAW);
    glVertexAttribPointer(glGetAttribLocation(program, "in_Position"), 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(glGetAttribLocation(program, "in_Position"));

    // Texturen
    glGenBuffers(1, &groundTexCoordBufferObjID);
    glBindBuffer(GL_ARRAY_BUFFER, groundTexCoordBufferObjID);
    glBufferData(GL_ARRAY_BUFFER, 12*sizeof(GLfloat), ground_tex_coord, GL_STATIC_DRAW);
    glVertexAttribPointer(glGetAttribLocation(program, "in_TexCoord"), 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(glGetAttribLocation(program, "in_TexCoord"));

    // Normalen till ground
    glBindVertexArray(gNormalArrayObjID);
    glGenBuffers(1, &gNormalBufferObjID);


    glBindBuffer(GL_ARRAY_BUFFER, gNormalBufferObjID);
    glBufferData(GL_ARRAY_BUFFER, 6*3*sizeof(GLfloat), gNormal, GL_STATIC_DRAW);
    glVertexAttribPointer(glGetAttribLocation(program, "in_Normal"), 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(glGetAttribLocation(program, "in_Normal"));
 
 
    // Draw the ground
    glBindVertexArray(groundVertexArrayObjID);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    printError("Display ground");
    

    

    i = 3; // Sluta rita med textures
    glUniform1i(glGetUniformLocation(program, "i"), i);
    i = 0; // Anv�nd i som en r�knare igen...
    
    // vingarna
    T(wing_x, wing_y, wing_z, trans);
    Ry(M_PI/24, total);
    Rx(t / 2, rot);

    Mult(rot, total, total);
    Mult(trans, total, total);
    glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total);
	

    DrawModel(wings, program,"in_Position","in_Normal", NULL);

    T(wing_x, wing_y, wing_z, trans);
    Ry(M_PI/24, total);
    Rx(M_PI/2 + t / 2, rot);

    Mult(rot, total, total);
    Mult(trans, total, total);
    glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total);
	

    DrawModel(wings, program,"in_Position","in_Normal", NULL);
  
    T(wing_x, wing_y, wing_z, trans);
    Ry(M_PI/24, total);
    Rx(M_PI +  t / 2, rot);

    Mult(rot, total, total);
    Mult(trans, total, total);
    glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total);
	

    DrawModel(wings, program,"in_Position","in_Normal", NULL);
  
    T(wing_x, wing_y, wing_z, trans);
    Ry(M_PI/24, total);
    Rx(3*M_PI/2 + t / 2, rot);

    Mult(rot, total, total);
    Mult(trans, total, total);
    glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total);
	

    DrawModel(wings, program,"in_Position","in_Normal", NULL);

    // M�llan
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

    // Draw a bunny, with multiple textures!!!
    
    i = 2;
    glUniform1i(glGetUniformLocation(program, "i"), i);
    T(3, -7, -35, trans);
    Ry(t, rot);
    
    Mult(trans, rot, total);
    glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total);
    DrawModel(bunny, program, "in_Position", "in_Normal", "in_TexCoord");
    printError("display");
    glutSwapBuffers();
  }

  int main(int argc, char *argv[])
  {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutCreateWindow ("Laboration 3! Keyboard Control!");
    glutDisplayFunc(&display); 
    //    glutPassiveMotionFunc(&MouseMove);
    initKeymapManager();
    glutTimerFunc(20, &OnTimer, 0);
    init ();
    glutMainLoop();
  }
