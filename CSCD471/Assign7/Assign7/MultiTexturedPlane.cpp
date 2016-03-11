#include <GL/glew.h>
#include <GL/freeglut.h>
#include <fstream>
#include <iostream>
#include <math.h>
#include <IL/il.h>
#include <IL/ilu.h>
#include <IL/ilut.h>
#include <cfloat>

#define GLM_FORCE_RADIANS 
#define GLM_FORCE_CXX11

#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective

#include "shader.h"
#include "shape.h"
#include "objloader.h"
#include "FrameBufferHandler.h"
#include "GlobalDefines.h"
#include "Screen.h"

using namespace glm;
using namespace std;

mat4 view;
mat4 model;
mat4 projection;
mat4 model_view;
mat4 transformation_matrix;
mat4 mvp;
mat3 normalmatrix;
static double gCameraScale = 1.0;
static double gCameraTranslationX = 0;
static double gCameraTranslationY = 0;
static bool gIsRotatingCamera = false;
static bool gIsScalingCamera = false;
static bool gIsTranslatingCamera = false;
static int gLastMouseX, gLastMouseY;
mat4 gCameraRotation;
GLuint program;
GLuint fboProg;
float aspect = 0.0;
GLfloat g_angle = 0.0;

GLuint texIDOne;
GLuint bunnyNormalMapID;

Shape bun;
Screen screen;

void Initialize();
void Display(void);
void makeIdentity();
void Reshape(int width, int height);
void glutMouse(int button, int state, int x, int y);
void glutMotion(int x, int y);
double projectToTrackball(double radius, double x, double y);
unsigned int loadTexture(string filename);
void setMatrices();

/************************************/
unsigned int loadTexture(string filename) {

	ILboolean success;
	unsigned int imageID;

	// Load Texture Map
	ilGenImages(1, &imageID);

	ilBindImage(imageID); /* Binding of DevIL image name */
	ilEnable(IL_ORIGIN_SET);
	ilOriginFunc(IL_ORIGIN_LOWER_LEFT);
	success = ilLoadImage(filename.c_str());

	if (!success) {
		cout << "Couldn't load the following texture file: " << filename << endl;
		// The operation was not sucessfull hence free image and texture 
		ilDeleteImages(1, &imageID);
		return 0;
	}

	// add information to the log
	ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);

	GLuint tid;
	glGenTextures(1, &tid);
	glBindTexture(GL_TEXTURE_2D, tid);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT), 0,
		GL_RGBA, GL_UNSIGNED_BYTE, ilGetData());

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, 0);

	/* Because we have already copied image data into texture data
	we can release memory used by image. */
	ilDeleteImages(1, &imageID);
	return tid;
}

/************************************/
void Initialize(void){
	// Create the program for rendering the model
	
	// Use our shader
	string bunnyFile = "bunny2.obj";
	objLoader loader(bunnyFile);
	//loader.print_data();
    bun = Shape(loader.getVertices(), loader.getTextures(), loader.getIndices());

	view = lookAt(vec3(0.0f, 0.0f, 3.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
	projection = mat4(1.0f);
    //program = LoadShaders("texture.vert", "texture.frag");
    program = LoadShaders("texture.vert", "texture.frag","texture.geom");
	fboProg = LoadShaders("framebuffer.vert", "framebuffer.frag");

	glUseProgram(program);
	
	vec3 light_intensity(1.0f, 1.0f, 1.0f);
	vec4 light_position(10.0f, 10.0f, 10.0f, 1.0f);
	vec3 material_ambient(0.5, 0.5, 0.5);
	vec3 material_diffuse(0.9, 0.9, 0.9);
	vec3 material_specular(0.9, 0.9, 0.9);

    GLfloat shininess = 100.0f;

    glUniform3fv(glGetUniformLocation(program, "Light.Intensity"), 1, reinterpret_cast<GLfloat*>(&light_intensity));
    glUniform4fv(glGetUniformLocation(program, "Light.Position"), 1, reinterpret_cast<GLfloat*>(&light_position));
	glUniform3fv(glGetUniformLocation(program, "Material.Ka"), 1, reinterpret_cast<GLfloat*>(&material_ambient));
	glUniform3fv(glGetUniformLocation(program, "Material.Kd"), 1, static_cast<GLfloat*>(&material_diffuse[0]));
	glUniform3fv(glGetUniformLocation(program, "Material.Ks"), 1, static_cast<GLfloat*>(&material_specular[0]));
	glUniform1f(glGetUniformLocation(program, "Material.Shininess"), shininess);
	
	texIDOne = loadTexture("test3.dds");
	glUniform1i(glGetUniformLocation(program, "Tex1"), 0);
	bunnyNormalMapID = loadTexture("NormalMap.bmp");
	glUniform1i(glGetUniformLocation(program, "NormalMap"), 1);

	screen.initScreen();
	
	glEnable(GL_DEPTH_TEST);

	glClearColor(1.0, 1.0, 1.0, 1.0);
}
/****************************************************************************/
void Display(void)
{
	//render scene to screen fbo
    screen.bindFBO();
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(program);

	transformation_matrix = mat4(1.0f);
	mat4 scaled = scale(mat4(1.0f), vec3(gCameraScale, gCameraScale, gCameraScale));
	mat4 translated = translate(mat4(1.0f), vec3(gCameraTranslationX, gCameraTranslationY, 0.0));
	transformation_matrix = translated*gCameraRotation*scaled;
	
	model = mat4(1.0f);
	
	setMatrices();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texIDOne);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, bunnyNormalMapID);
	bun.render();
    screen.unbindFBO();

	screen.render();

	glutSwapBuffers();
}

/******************************************************************************/
void setMatrices(){
	model *= transformation_matrix;
    normalmatrix = mat3(1.0f);
    mvp = mat4(1.0f);
	model_view = view*model;
    normalmatrix = mat3(transpose(inverse(model_view)));
	projection = perspective(70.0f, aspect, 0.3f, 100.0f);
	mvp = projection*model_view;

	glUniformMatrix4fv(glGetUniformLocation(program, "ModelViewMatrix"), 1, GL_FALSE, reinterpret_cast<GLfloat*>(&model_view[0]));
	glUniformMatrix3fv(glGetUniformLocation(program, "NormalMatrix"), 1, GL_FALSE, reinterpret_cast<GLfloat*>(&normalmatrix[0]));
	glUniformMatrix4fv(glGetUniformLocation(program, "MVP"), 1, GL_FALSE, reinterpret_cast<GLfloat*>(&mvp[0]));
	glUniformMatrix4fv(glGetUniformLocation(program, "ProjectionMatrix"), 1, GL_FALSE, reinterpret_cast<GLfloat*>(&projection[0]));
}

/********************************************************************************/

void Reshape(int width, int height)
{
	screen.setViewportWidth(width);
	screen.setViewportHeight(height);
	glViewport(0, 0, width, height);

	//projection = glm::perspective(50.0f, (float)width / height, 0.3f, 1000.0f);
	aspect = float(width) / float(height);
}
/*****************************************************************************/

// ReSharper disable CppParameterNeverUsed
#ifdef _WIN32
#pragma warning(push)
#pragma warning(disable: 4100)
#endif
#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#endif
void keyboard(unsigned char key, int x, int y)
{
	// ReSharper restore CppParameterNeverUsed
	switch (key) {
	case 'q':case 'Q':
		exit(EXIT_SUCCESS);
		break;
	case 'r':
	case 'R':
		makeIdentity();
		break;

	}
	glutPostRedisplay();
}
#ifdef _WIN32
#pragma warning(pop)
#endif
#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif

/*********************************************************************************************/
void makeIdentity(){

	gCameraScale = 1.0;
	gCameraTranslationX = 0;
	gCameraTranslationY = 0;
	gCameraRotation = mat4(1.0f);
}
/************************************************************************************************/
double projectToTrackball(double radius, double x, double y)
{
    static const double Radius = std::sqrt(2.0);
	double z;

    double dist = std::sqrt(x * x + y * y);
	if (dist < radius * Radius / 2.0)
	{
		// Solve for sphere case.
        z = std::sqrt(radius * radius - dist * dist);
	}
	else
	{
		// Solve for hyperbolic sheet case.
		double t = radius / Radius;
		z = t * t / dist;
	}

	return z;
}

/*******************************************************************************/
/******************************************************************************
GLUT callback for responding to mouse button presses. Detecst whether to
initiate a point snapping, view rotation or view scale.
******************************************************************************/
void glutMouse(int button, int state, int x, int y)
{
	if (state == GLUT_DOWN)
	{
		if (button == GLUT_LEFT_BUTTON)
		{
			gIsRotatingCamera = true;
		}
		else if (button == GLUT_MIDDLE_BUTTON)
		{
			gIsTranslatingCamera = true;
		}
		else if (button == GLUT_RIGHT_BUTTON)
		{

			gIsScalingCamera = true;
		}

		gLastMouseX = x;
		gLastMouseY = y;
	}
	else
	{
		gIsRotatingCamera = false;
		gIsScalingCamera = false;
		gIsTranslatingCamera = false;
	}
}
/**************************************************************************/
/*******************************************************************************
*******************************************************************************/

void glutMotion(int x, int y)
{
	if (gIsRotatingCamera)
	{
		static const double kTrackBallRadius = 0.8;
		vec3 lastPos;
		vec3 currPos;

		lastPos.x = gLastMouseX * 2.0f / screen.getViewportWidth()- 1.0f;
		lastPos.y = (screen.getViewportHeight() - gLastMouseY) * 2.0f / screen.getViewportHeight() - 1.0f;
		lastPos.z = static_cast<float>(projectToTrackball(kTrackBallRadius, lastPos[0], lastPos[1]));

		currPos.x = x * 2.0f / screen.getViewportWidth() - 1.0f;
		currPos.y = (screen.getViewportHeight() - y) * 2.0f / screen.getViewportHeight() - 1.0f;
		currPos.z = static_cast<float>(projectToTrackball(kTrackBallRadius, currPos[0], currPos[1]));

		currPos = normalize(currPos);
		lastPos = normalize(lastPos);

		vec3 rotateVecCross;

		rotateVecCross = cross(vec3(lastPos), vec3(currPos));

		double sin_rotate_angle = std::sqrt(rotateVecCross.x * rotateVecCross.x + rotateVecCross.y * rotateVecCross.y + rotateVecCross.z * rotateVecCross.z);
		double angle = asin(std::sqrt(rotateVecCross.x * rotateVecCross.x + rotateVecCross.y * rotateVecCross.y + rotateVecCross.z * rotateVecCross.z));
		double cos_rotate_angle = cos(angle);

		rotateVecCross = normalize(rotateVecCross);

		mat4 deltaRotation(1.0f);

		deltaRotation[0][0] = static_cast<float>(cos_rotate_angle + rotateVecCross[0] * rotateVecCross[0] * (1 - cos_rotate_angle));
		deltaRotation[0][1] = static_cast<float>(rotateVecCross[0] * rotateVecCross[1] * (1 - cos_rotate_angle) + rotateVecCross[2] * sin_rotate_angle);
		deltaRotation[0][2] = static_cast<float>(rotateVecCross[0] * rotateVecCross[2] * (1 - cos_rotate_angle) - rotateVecCross[1] * sin_rotate_angle);
		deltaRotation[0][3] = 0.0f;
		deltaRotation[1][0] = static_cast<float>(rotateVecCross[0] * rotateVecCross[1] * (1 - cos_rotate_angle) - rotateVecCross[2] * sin_rotate_angle);
		deltaRotation[1][1] = static_cast<float>(cos_rotate_angle + rotateVecCross[1] * rotateVecCross[1] * (1 - cos_rotate_angle));
		deltaRotation[1][2] = static_cast<float>(rotateVecCross[2] * rotateVecCross[1] * (1 - cos_rotate_angle) + rotateVecCross[0] * sin_rotate_angle);
		deltaRotation[1][3] = 0.0f;
		deltaRotation[2][0] = static_cast<float>(rotateVecCross[0] * rotateVecCross[2] * (1 - cos_rotate_angle) + rotateVecCross[1] * sin_rotate_angle);
		deltaRotation[2][1] = static_cast<float>(rotateVecCross[1] * rotateVecCross[2] * (1 - cos_rotate_angle) - rotateVecCross[0] * sin_rotate_angle);
		deltaRotation[2][2] = static_cast<float>(cos_rotate_angle + rotateVecCross[2] * rotateVecCross[2] * (1 - cos_rotate_angle));

		mat4 tempMatrix;

		if (abs(angle) > DBL_EPSILON*20.0){
			tempMatrix = gCameraRotation*deltaRotation;
			gCameraRotation = tempMatrix;
		}
	}
	if (gIsTranslatingCamera)
	{
		gCameraTranslationX += 2 * double(x - gLastMouseX) / screen.getViewportWidth();
		gCameraTranslationY -= 2 * double(y - gLastMouseY) / screen.getViewportHeight();

	}
	else if (gIsScalingCamera)
	{
		float y1 = static_cast<float>(screen.getViewportHeight() - gLastMouseY);
		float y2 = static_cast<float>(screen.getViewportHeight() - y);

		gCameraScale *= 1 + (y1 - y2) / screen.getViewportHeight();
	}

	glutPostRedisplay();

	gLastMouseX = x;
	gLastMouseY = y;
}
/************************************************************************************************************************************************************/
/*********/
int main(int argc, char** argv)
{
    glewExperimental = GL_TRUE;
    glutInitContextVersion(3,3);

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);

	glutCreateWindow("Geometry Bunny");

	if (glewInit()){
        cerr << "Unable to initialize GLEW ... exiting" << endl;
	}

	ilInit();
	Initialize();
	cout << glGetString(GL_VERSION) << endl;
	glutDisplayFunc(Display);
	glutMotionFunc(glutMotion);
	glutMouseFunc(glutMouse);
	glutKeyboardFunc(keyboard);
	glutReshapeFunc(Reshape);
	glutMainLoop();
	return 0;
}


