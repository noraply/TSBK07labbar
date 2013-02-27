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

void calculate_normals( int x, int z, int width, int height, GLfloat *vertex_arr, GLfloat **norm_arr)
{
    Point3D res, temp_res;
    Point3D tmp1, tmp2, tmp3, tmp4, tmp5, tmp6, tmp7;
    Point3D vec1, vec2, vec3, vec4, vec5, vec6;
    
    GLfloat *normals = *norm_arr;
    /* Fall 1 */
    if(x == 1 && z == 1){
        printf("Fall 1\n");
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
        printf("Fall 2\n");
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
        printf("Fall 4\n");
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
        printf("Fall 5\n");
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
        printf("Normal Vector is (%f,%f,%f)\n", res.x, res.y, res.z);
        printf("(x,z) =  (%i, %i)\n", x, z);

        normals[(x + (z - 1)*width)*3 + 0] = res.x;
        normals[(x + (z - 1)*width)*3 + 1] = res.y;
        normals[(x + (z - 1)*width)*3 + 2] = res.z;

    /* Fall 6 */
    }
    if( x ==  width -1 && z > 1){
        printf("Fall 6\n");
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

        printf("(x,z - 1) från (%i,%i)\n", x, z);
        normals[(x + (z - 1)*width)*3 + 0] = res.x;
        normals[(x + (z - 1)*width)*3 + 1] = res.y;
        normals[(x + (z - 1)*width)*3 + 2] = res.z;

    /* Fall 8 */
    }
    if(z == height -1 && x > 1){
        printf("Fall 8\n");
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

        printf("(x - 1,z) från (%i,%i)\n", x, z);
        normals[((x-1) + z*width)*3 + 0] = res.x;
        normals[((x-1) + z*width)*3 + 1] = res.y;
        normals[((x-1) + z*width)*3 + 2] = res.z;

    }
    
/* Fall 3 */
if(x == 1 && z > 1){
        printf("Fall 3\n");
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
        printf("Fall 7\n");
        tmp1.x = vertex_arr[(x + (z-1)*width)*3 + 0];
        tmp1.y = vertex_arr[(x + (z-1)*width)*3 + 1];
        tmp1.z = vertex_arr[(x + (z-1)*width)*3 + 2];

        tmp3.x = vertex_arr[(x + z*width)*3 + 0];
        tmp3.y = vertex_arr[(x + z*width)*3 + 1];
        tmp3.z = vertex_arr[(x + z*width)*3 + 2];
        
        tmp2.x = vertex_arr[((x-1) + z*width)*3 + 0];
        tmp2.y = vertex_arr[((x-1) + z*width)*3 + 1];
        tmp2.z = vertex_arr[((x-1) + z*width)*3 + 2];

        VectorSub(&tmp1, &tmp2, &vec1);
        VectorSub(&tmp3, &tmp2, &vec2);

        CrossProduct(&vec1, &vec2, &res);

        Normalize(&res);
        printf("(x,z) från (%i,%i)\n", x, z);
        normals[(x + z*width)*3 + 0] = res.x;
        normals[(x + z*width)*3 + 1] = res.y;
        normals[(x + z*width)*3 + 2] = res.z;
}

/* Generella fallet */
if(x > 1 && z > 1) {
    printf("Fall generellt!\n");
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

    printf("(x - 1,z - 1) från (%i,%i)\n", x, z);
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
    cam.y = 2;
    cam.z = 0;
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
