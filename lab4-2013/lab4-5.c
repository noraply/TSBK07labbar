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

#define CAM_HEIGHT 3
GLfloat projectionMatrix[16];

GLfloat rot[16], trans[16], total[16], camMatrix[16], cam_mod[16];
GLfloat t = 0;
GLfloat i = 0;
Point3D cam = {114.0, 25.5 + CAM_HEIGHT, 33.0};
Point3D lookAtPoint = {116.0, 25.5, 33.0};
Point3D up;
GLboolean fly = false;

Model *sphere;

int terrain_height, terrain_width;

float calculate_height(Model *ground, Point3D pos);

void calculate_normals( int x, int z, int width, int height, GLfloat *vertex_arr, GLfloat **norm_arr)
{
    Point3D res, temp_res;
    Point3D tmp1, tmp2, tmp3, tmp4, tmp5, tmp6, tmp7;
    Point3D vec1, vec2, vec3, vec4, vec5, vec6;
    
    GLfloat *normals = *norm_arr;
    /* Fall 1 */
    if(x == 1 && z == 1){
        tmp3.x = vertex_arr[(x + (z-1)*width)*3 + 0];
        tmp3.y = vertex_arr[(x + (z-1)*width)*3 + 1];
        tmp3.z = vertex_arr[(x + (z-1)*width)*3 + 2];

        tmp2.x = vertex_arr[((x-1) + (z-1)*width)*3 + 0];
        tmp2.y = vertex_arr[((x-1) + (z-1)*width)*3 + 1];
        tmp2.z = vertex_arr[((x-1) + (z-1)*width)*3 + 2];
        
        tmp1.x = vertex_arr[((x-1) + z*width)*3 + 0];
        tmp1.y = vertex_arr[((x-1) + z*width)*3 + 1];
        tmp1.z = vertex_arr[((x-1) + z*width)*3 + 2];

        VectorSub(&tmp1, &tmp2, &vec1);
        VectorSub(&tmp3, &tmp2, &vec2);

        CrossProduct(&vec1, &vec2, &res);

        Normalize(&res);
        normals[((x-1) + (z-1)*width)*3 + 0] = res.x;
        normals[((x-1) + (z-1)*width)*3 + 1] = res.y;
        normals[((x-1) + (z-1)*width)*3 + 2] = res.z;


    /* Fall 2 */
    }
    if(z == 0 && x > 1){
        tmp1.x = vertex_arr[((x-2) + z*width)*3 + 0];
        tmp1.y = vertex_arr[((x-2) + z*width)*3 + 1];
        tmp1.z = vertex_arr[((x-2) + z*width)*3 + 2];

        tmp2.x = vertex_arr[((x - 1) + z*width)*3 + 0];
        tmp2.y = vertex_arr[((x - 1) + z*width)*3 + 1];
        tmp2.z = vertex_arr[((x - 1) + z*width)*3 + 2];
        
        tmp3.x = vertex_arr[((x-2) + (z + 1)*width)*3 + 0];
        tmp3.y = vertex_arr[((x-2) + (z + 1)*width)*3 + 1];
        tmp3.z = vertex_arr[((x-2) + (z + 1)*width)*3 + 2];

        tmp4.x = vertex_arr[((x-1) + (z + 1)*width)*3 + 0];
        tmp4.y = vertex_arr[((x-1) + (z + 1)*width)*3 + 1];
        tmp4.z = vertex_arr[((x-1) + (z + 1)*width)*3 + 2];
        
        tmp5.x = vertex_arr[(x + z*width)*3 + 0];
        tmp5.y = vertex_arr[(x + z*width)*3 + 1];
        tmp5.z = vertex_arr[(x + z*width)*3 + 2];

        VectorSub(&tmp1, &tmp2, &vec1);
        VectorSub(&tmp3, &tmp2, &vec2);
        VectorSub(&tmp4, &tmp2, &vec3);
        VectorSub(&tmp5, &tmp2, &vec4);

        CrossProduct(&vec1, &vec2, &temp_res);
        res = temp_res;

        CrossProduct(&vec2, &vec3, &temp_res);
        VectorAdd(&res, &temp_res, &res);
        
        CrossProduct(&vec3, &vec4, &temp_res);
        VectorAdd(&res, &temp_res, &res);

        Normalize(&res);

        normals[((x-1) + z*width)*3 + 0] = res.x;
        normals[((x-1) + z*width)*3 + 1] = res.y;
        normals[((x-1) + z*width)*3 + 2] = res.z;
    /* Fall 4 */
    }
    if(x == 1 && z == height -1){
        tmp1.x = vertex_arr[(x + z*width)*3 + 0];
        tmp1.y = vertex_arr[(x + z*width)*3 + 1];
        tmp1.z = vertex_arr[(x + z*width)*3 + 2];

        tmp2.x = vertex_arr[((x - 1) + z*width)*3 + 0];
        tmp2.y = vertex_arr[((x - 1) + z*width)*3 + 1];
        tmp2.z = vertex_arr[((x - 1) + z*width)*3 + 2];
        
        tmp3.x = vertex_arr[(x + (z - 1)*width)*3 + 0];
        tmp3.y = vertex_arr[(x + (z - 1)*width)*3 + 1];
        tmp3.z = vertex_arr[(x + (z - 1)*width)*3 + 2];

        tmp4.x = vertex_arr[((x-1) + (z - 1)*width)*3 + 0];
        tmp4.y = vertex_arr[((x-1) + (z - 1)*width)*3 + 1];
        tmp4.z = vertex_arr[((x-1) + (z - 1)*width)*3 + 2];
        
        VectorSub(&tmp1, &tmp2, &vec1);
        VectorSub(&tmp3, &tmp2, &vec2);
        VectorSub(&tmp4, &tmp2, &vec3);

        CrossProduct(&vec1, &vec2, &temp_res);
        res = temp_res;

        CrossProduct(&vec2, &vec3, &temp_res);
        VectorAdd(&res, &temp_res, &res);
        

        Normalize(&res);

        normals[((x-1) + z*width)*3 + 0] = res.x;
        normals[((x-1) + z*width)*3 + 1] = res.y;
        normals[((x-1) + z*width)*3 + 2] = res.z;

    /* Fall 5 */
    }
    if(x == width - 1  && z == 1){
        tmp4.x = vertex_arr[(x + z*width)*3 + 0];
        tmp4.y = vertex_arr[(x + z*width)*3 + 1];
        tmp4.z = vertex_arr[(x + z*width)*3 + 2];

        tmp3.x = vertex_arr[((x - 1) + z*width)*3 + 0];
        tmp3.y = vertex_arr[((x - 1) + z*width)*3 + 1];
        tmp3.z = vertex_arr[((x - 1) + z*width)*3 + 2];
        
        tmp2.x = vertex_arr[(x + (z - 1)*width)*3 + 0];
        tmp2.y = vertex_arr[(x + (z - 1)*width)*3 + 1];
        tmp2.z = vertex_arr[(x + (z - 1)*width)*3 + 2];

        tmp1.x = vertex_arr[((x - 1) + (z - 1)*width)*3 + 0];
        tmp1.y = vertex_arr[((x - 1) + (z - 1)*width)*3 + 1];
        tmp1.z = vertex_arr[((x - 1) + (z - 1)*width)*3 + 2];
        
        VectorSub(&tmp1, &tmp2, &vec1);
        VectorSub(&tmp3, &tmp2, &vec2);
        VectorSub(&tmp4, &tmp2, &vec3);

        CrossProduct(&vec1, &vec2, &res);

        CrossProduct(&vec2, &vec3, &temp_res);
        VectorAdd(&res, &temp_res, &res);
        

        Normalize(&res);

        normals[(x + (z - 1)*width)*3 + 0] = res.x;
        normals[(x + (z - 1)*width)*3 + 1] = res.y;
        normals[(x + (z - 1)*width)*3 + 2] = res.z;

    /* Fall 6 */
    }
    if( x ==  width -1 && z > 1){
        
        tmp4.x = vertex_arr[((x-1) + z*width)*3 + 0];
        tmp4.y = vertex_arr[((x-1) + z*width)*3 + 1];
        tmp4.z = vertex_arr[((x-1) + z*width)*3 + 2];

        tmp3.x = vertex_arr[((x - 1) + (z - 1)*width)*3 + 0];
        tmp3.y = vertex_arr[((x - 1) + (z - 1)*width)*3 + 1];
        tmp3.z = vertex_arr[((x - 1) + (z - 1)*width)*3 + 2];
        
        tmp2.x = vertex_arr[(x + (z - 1)*width)*3 + 0];
        tmp2.y = vertex_arr[(x + (z - 1)*width)*3 + 1];
        tmp2.z = vertex_arr[(x + (z - 1)*width)*3 + 2];

        tmp1.x = vertex_arr[(x + (z - 2)*width)*3 + 0];
        tmp1.y = vertex_arr[(x + (z - 2)*width)*3 + 1];
        tmp1.z = vertex_arr[(x + (z - 2)*width)*3 + 2];
        
        tmp5.x = vertex_arr[(x + z*width)*3 + 0];
        tmp5.y = vertex_arr[(x + z*width)*3 + 1];
        tmp5.z = vertex_arr[(x + z*width)*3 + 2];
        
        VectorSub(&tmp1, &tmp2, &vec1);
        VectorSub(&tmp3, &tmp2, &vec2);
        VectorSub(&tmp4, &tmp2, &vec3);
        VectorSub(&tmp5, &tmp2, &vec4);

        CrossProduct(&vec1, &vec2, &temp_res);
        res = temp_res;

        CrossProduct(&vec2, &vec3, &temp_res);
        VectorAdd(&res, &temp_res, &res);
        
        CrossProduct(&vec3, &vec4, &temp_res);
        VectorAdd(&res, &temp_res, &res);

        Normalize(&res);

        normals[(x + (z - 1)*width)*3 + 0] = res.x;
        normals[(x + (z - 1)*width)*3 + 1] = res.y;
        normals[(x + (z - 1)*width)*3 + 2] = res.z;

    /* Fall 8 */
    }
    if(z == height -1 && x > 1){
        
        tmp5.x = vertex_arr[((x-2) + z*width)*3 + 0];
        tmp5.y = vertex_arr[((x-2) + z*width)*3 + 1];
        tmp5.z = vertex_arr[((x-2) + z*width)*3 + 2];

        tmp2.x = vertex_arr[((x - 1) + z*width)*3 + 0];
        tmp2.y = vertex_arr[((x - 1) + z*width)*3 + 1];
        tmp2.z = vertex_arr[((x - 1) + z*width)*3 + 2];
        
        tmp3.x = vertex_arr[(x + (z - 1)*width)*3 + 0];
        tmp3.y = vertex_arr[(x + (z - 1)*width)*3 + 1];
        tmp3.z = vertex_arr[(x + (z - 1)*width)*3 + 2];

        tmp4.x = vertex_arr[((x-1) + (z - 1)*width)*3 + 0];
        tmp4.y = vertex_arr[((x-1) + (z - 1)*width)*3 + 1];
        tmp4.z = vertex_arr[((x-1) + (z - 1)*width)*3 + 2];
        
        tmp1.x = vertex_arr[(x + z*width)*3 + 0];
        tmp1.y = vertex_arr[(x + z*width)*3 + 1];
        tmp1.z = vertex_arr[(x + z*width)*3 + 2];

        VectorSub(&tmp1, &tmp2, &vec1);
        VectorSub(&tmp3, &tmp2, &vec2);
        VectorSub(&tmp4, &tmp2, &vec3);
        VectorSub(&tmp5, &tmp2, &vec4);

        CrossProduct(&vec1, &vec2, &temp_res);
        res = temp_res;

        CrossProduct(&vec2, &vec3, &temp_res);
        VectorAdd(&res, &temp_res, &res);
        
        CrossProduct(&vec3, &vec4, &temp_res);
        VectorAdd(&res, &temp_res, &res);

        Normalize(&res);
        normals[((x-1) + z*width)*3 + 0] = res.x;
        normals[((x-1) + z*width)*3 + 1] = res.y;
        normals[((x-1) + z*width)*3 + 2] = res.z;

    }
    
/* Fall 3 */
if(x == 1 && z > 1){
        tmp1.x = vertex_arr[((x-1) + z*width)*3 + 0];
        tmp1.y = vertex_arr[((x-1) + z*width)*3 + 1];
        tmp1.z = vertex_arr[((x-1) + z*width)*3 + 2];

        tmp2.x = vertex_arr[((x - 1) + (z - 1)*width)*3 + 0];
        tmp2.y = vertex_arr[((x - 1) + (z - 1)*width)*3 + 1];
        tmp2.z = vertex_arr[((x - 1) + (z - 1)*width)*3 + 2];
        
        tmp3.x = vertex_arr[(x + (z - 1)*width)*3 + 0];
        tmp3.y = vertex_arr[(x + (z - 1)*width)*3 + 1];
        tmp3.z = vertex_arr[(x + (z - 1)*width)*3 + 2];

        tmp4.x = vertex_arr[(x + (z - 2)*width)*3 + 0];
        tmp4.y = vertex_arr[(x + (z - 2)*width)*3 + 1];
        tmp4.z = vertex_arr[(x + (z - 2)*width)*3 + 2];
        
        tmp5.x = vertex_arr[((x - 1) + (z - 2)*width)*3 + 0];
        tmp5.y = vertex_arr[((x - 1) + (z - 2)*width)*3 + 1];
        tmp5.z = vertex_arr[((x - 1) + (z - 2)*width)*3 + 2];
        
        VectorSub(&tmp1, &tmp2, &vec1);
        VectorSub(&tmp3, &tmp2, &vec2);
        VectorSub(&tmp4, &tmp2, &vec3);
        VectorSub(&tmp5, &tmp2, &vec4);

        CrossProduct(&vec1, &vec2, &temp_res);
        res = temp_res;

        CrossProduct(&vec2, &vec3, &temp_res);
        VectorAdd(&res, &temp_res, &res);
        
        CrossProduct(&vec3, &vec4, &temp_res);
        VectorAdd(&res, &temp_res, &res);

        Normalize(&res);

        normals[((x-1) + (z - 1)*width)*3 + 0] = res.x;
        normals[((x-1) + (z - 1)*width)*3 + 1] = res.y;
        normals[((x-1) + (z - 1)*width)*3 + 2] = res.z;
}

/* Fall 7 */
if(x == width -1 && z == height -1){
        tmp1.x = vertex_arr[(x + (z-1)*width)*3 + 0];
        tmp1.y = vertex_arr[(x + (z-1)*width)*3 + 1];
        tmp1.z = vertex_arr[(x + (z-1)*width)*3 + 2];

        tmp3.x = vertex_arr[((x-1) + z*width)*3 + 0];
        tmp3.y = vertex_arr[((x-1) + z*width)*3 + 1];
        tmp3.z = vertex_arr[((x-1) + z*width)*3 + 2];
        
        tmp2.x = vertex_arr[(x + z*width)*3 + 0];
        tmp2.y = vertex_arr[(x + z*width)*3 + 1];
        tmp2.z = vertex_arr[(x + z*width)*3 + 2];

        VectorSub(&tmp1, &tmp2, &vec1);
        VectorSub(&tmp3, &tmp2, &vec2);

        CrossProduct(&vec1, &vec2, &res);

        Normalize(&res);
        normals[(x + z*width)*3 + 0] = res.x;
        normals[(x + z*width)*3 + 1] = res.y;
        normals[(x + z*width)*3 + 2] = res.z;
}

/* Generella fallet */
if(x > 1 && z > 1) {
    tmp1.x = vertex_arr[((x-1) + z*width)*3 + 0];
    tmp1.y = vertex_arr[((x-1) + z*width)*3 + 1];
    tmp1.z = vertex_arr[((x-1) + z*width)*3 + 2];

    tmp2.x = vertex_arr[((x - 1) + (z - 1)*width)*3 + 0];
    tmp2.y = vertex_arr[((x - 1) + (z - 1)*width)*3 + 1];
    tmp2.z = vertex_arr[((x - 1) + (z - 1)*width)*3 + 2];

    tmp3.x = vertex_arr[(x + (z - 1)*width)*3 + 0];
    tmp3.y = vertex_arr[(x + (z - 1)*width)*3 + 1];
    tmp3.z = vertex_arr[(x + (z - 1)*width)*3 + 2];

    tmp4.x = vertex_arr[(x + (z - 2)*width)*3 + 0];
    tmp4.y = vertex_arr[(x + (z - 2)*width)*3 + 1];
    tmp4.z = vertex_arr[(x + (z - 2)*width)*3 + 2];

    tmp5.x = vertex_arr[((x - 1) + (z - 2)*width)*3 + 0];
    tmp5.y = vertex_arr[((x - 1) + (z - 2)*width)*3 + 1];
    tmp5.z = vertex_arr[((x - 1) + (z - 2)*width)*3 + 2];

    tmp6.x = vertex_arr[((x - 2) + (z - 1)*width)*3 + 0];
    tmp6.y = vertex_arr[((x - 2) + (z - 1)*width)*3 + 1];
    tmp6.z = vertex_arr[((x - 2) + (z - 1)*width)*3 + 2];

    tmp7.x = vertex_arr[((x - 2) + z*width)*3 + 0];
    tmp7.y = vertex_arr[((x - 2) + z*width)*3 + 1];
    tmp7.z = vertex_arr[((x - 2) + z*width)*3 + 2];


    VectorSub(&tmp1, &tmp2, &vec1);
    VectorSub(&tmp3, &tmp2, &vec2);
    VectorSub(&tmp4, &tmp2, &vec3);
    VectorSub(&tmp5, &tmp2, &vec4);
    VectorSub(&tmp6, &tmp2, &vec5);
    VectorSub(&tmp7, &tmp2, &vec6);

    CrossProduct(&vec1, &vec2, &temp_res);
    res = temp_res;

    CrossProduct(&vec2, &vec3, &temp_res);
    VectorAdd(&res, &temp_res, &res);

    CrossProduct(&vec3, &vec4, &temp_res);
    VectorAdd(&res, &temp_res, &res);

    CrossProduct(&vec4, &vec5, &temp_res);
    VectorAdd(&res, &temp_res, &res);

    CrossProduct(&vec5, &vec6, &temp_res);
    VectorAdd(&res, &temp_res, &res);

    CrossProduct(&vec6, &vec1, &temp_res);
    VectorAdd(&res, &temp_res, &res);

    Normalize(&res);

    normals[((x - 1) + (z - 1)*width)*3 + 0] = res.x;
    normals[((x - 1) + (z - 1)*width)*3 + 1] = res.y;
    normals[((x - 1) + (z - 1)*width)*3 + 2] = res.z;

}
    norm_arr = &normals;
}

Model* GenerateTerrain(TextureData *tex)
{
	int vertexCount = tex->width * tex->height;
	int triangleCount = (tex->width-1) * (tex->height-1) * 2;
	int x, z;

    terrain_height = tex->height;
    terrain_width = tex->width;
	
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
            calculate_normals( x, z, tex->width, tex->height, vertexArray, &normalArray);

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
    free(vertexArray);
    free(indexArray);
    free(normalArray);

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

	frustum(-0.1, 0.1, -0.1, 0.1, 0.2, 500.0, projectionMatrix);

	// Load and compile shader
	program = loadShaders("terrain.vert", "terrain.frag");
	glUseProgram(program);
	printError("init shader");
	
	glUniformMatrix4fv(glGetUniformLocation(program, "projMatrix"), 1, GL_TRUE, projectionMatrix);
	glUniform1i(glGetUniformLocation(program, "tex"), 0); // Texture unit 0
	LoadTGATextureSimple("grass.tga", &tex1);
	
// Load terrain data
	
	LoadTGATexture("TERRA2.tga", &ttex);
	tm = GenerateTerrain(&ttex);
	printError("init terrain");

    sphere = LoadModelPlus("groundsphere.obj");

// light source
    GLfloat light[3] = {0, 22, 69};
    glUniform3fv(glGetUniformLocation(program, "light"), 1, light);
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
    cam.x = 0.0;
    cam.y = 2.0;
    cam.z = 0.0;
    lookAtPoint.z = 0;
    lookAtPoint.x = 0;
    lookAtPoint.y = 0;
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
  }else if(keyIsDown('c')){
    printf("(x,y,z) = (%f,%f,%f)\n", cam.x, calculate_height(tm, cam), cam.z);
  }else if(keyIsDown('f')){
      fly = !fly;
  }

  if(fly){
      cam.y = CAM_HEIGHT + calculate_height(tm, cam);
  }
}
int find_quad(Model *ground, Point3D pos)
{
    int x = floor(pos.x);
    int z = floor(pos.z);

    return (x + z*terrain_width)*3;
}

int find_triangle(Model *ground, Point3D pos)
{
    int x = floor(pos.x);
    int z = floor(pos.z);
    int diff = (pos.x - x + pos.z - z);

    /* Upper triangle */
    if(diff >= 1){
        return 1;
    }else{
        return 0;
    }
}

float calculate_height(Model *ground, Point3D pos)
{
    int quad = find_quad(ground, pos);
    int upper = find_triangle(ground, pos);

    GLfloat A,B,C,D;
    Point3D pt1, pt2, pt3, vec1, vec2, normal;

    pt1.x = ground->vertexArray[quad + 0];
    pt1.y = ground->vertexArray[quad + 1];
    pt1.z = ground->vertexArray[quad + 2];


    pt3.x = ground->vertexArray[quad + (1 + terrain_width)*3];
    pt3.y = ground->vertexArray[quad + (1 + terrain_width)*3 + 1];
    pt3.z = ground->vertexArray[quad + (1 + terrain_width)*3 + 2];

    if(upper){
      pt2.x = ground->vertexArray[quad + terrain_width*3];
      pt2.y = ground->vertexArray[quad + terrain_width*3 + 1];
      pt2.z = ground->vertexArray[quad + terrain_width*3 + 2];
    }else{
      pt2.x = ground->vertexArray[quad + 3];
      pt2.y = ground->vertexArray[quad + 3 + 1];
      pt2.z = ground->vertexArray[quad + 3 + 2];
    }

    VectorSub(&pt2, &pt1, &vec1);
    VectorSub(&pt3, &pt1, &vec2);

    CrossProduct(&vec1, &vec2, &normal);
    A = normal.x;
    B = normal.y;
    C = normal.z;
    D = A*pt1.x + B*pt1.y + C*pt1.z;

    GLfloat res = (D - A*pos.x - C*pos.z)/ B;

    return res;
}

void display(void)
{

	check_keyboard();
	// clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	GLfloat total[16], modelView[16], trans[16], rot[16];
	
	printError("pre display");
	
	glUseProgram(program);

	// Build matrix
	
//  cam.y = CAM_HEIGHT + calculate_height(tm, cam);
	lookAt(&cam,
				&lookAtPoint,
				0, 1, 0,
				camMatrix);
	IdentityMatrix(modelView);
	Mult(camMatrix, modelView, total);
	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total);
    glUniform1i(glGetUniformLocation(program, "textures"), 1);

	glBindTexture(GL_TEXTURE_2D, tex1);		// Bind Our Texture tex1
	DrawModel(tm, program, "inPosition", "inNormal", "inTexCoord");

    IdentityMatrix(rot);

    Point3D pos = { 65.0 + 18*cos(t), 25.5, 54.0 + 18*sin(t)};
    pos.y = calculate_height(tm, pos);



    T(pos.x, pos.y, pos.z, trans);
    Mult(trans, rot, total);
    Mult(camMatrix, total, total);
    glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total);

    
    glUniform1i(glGetUniformLocation(program, "textures"), 0);
    GLfloat color[4] = {0.5, 0.1, 0.85};
    glUniform4fv(glGetUniformLocation(program, "color"), 1, color);
    DrawModel(sphere, program, "inPosition", "inNormal", "inTexCoord");
	printError("display 2");
	
	glutSwapBuffers();
}

void timer(int i)
{
    t += 0.02;
	glutTimerFunc(20, &timer, i);
	glutPostRedisplay();
}

void mouse(int x, int y)
{
	GLfloat phi;
	GLfloat theta;
	phi = (float)(x)/glutGet(GLUT_WINDOW_WIDTH)*2*M_PI;
	theta = (float)(y)/glutGet(GLUT_WINDOW_HEIGHT)*2*M_PI;

	lookAtPoint.x = cam.x - 10*sin(theta)*sin(phi);
	lookAtPoint.y = cam.y + 10*cos(theta);
	lookAtPoint.z = cam.z + 10*sin(theta)*cos(phi);
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
