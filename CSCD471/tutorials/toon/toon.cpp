#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>
#include <math.h>
#include <vector>

#define GLM_FORCE_RADIANS 
#define TWOPI 2*3.1415926535897932384626433832795

#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective

#include "objloader.h"
#include "shader.h"

using namespace glm;
using namespace std;

mat4 view;
mat4 model;
mat4 projection;
mat4 model_view;

GLuint projection_matrix_loc;
GLuint model_view_matrix_loc;
GLuint program;
GLuint programOne;
float aspect = 0.0;
GLfloat angle = 0.0;

GLuint vao;

std::vector<glm::vec3> vertices;
std::vector<int> vIndices;

std::vector<glm::vec3> normals; // Won't be used at the moment.
vector<int> indices;
static const double kPI = 3.1415926535897932384626433832795;
static int gViewportWidth, gViewportHeight;
static double gCameraScale = 1.0;
static double gCameraTranslationX = 0;
static double gCameraTranslationY = 0;
static bool gIsRotatingCamera = false;
static bool gIsScalingCamera = false;
static bool gIsTranslatingCamera = false;
static int gLastMouseX, gLastMouseY;
mat4 gCameraRotation;



void Initialize();
void display(void);
void makeIdentity();
void Reshape(int width, int height);
void glutMouse(int button, int state, int x, int y);
void glutMotion(int x, int y);
double projectToTrackball(double radius, double x, double y);



void Initialize(void){
	// Create the program for rendering the model

	// Read our .obj file
	
	OBJLoader loader;

	bool loadfile = loader.load("bunny.obj");
	vertices = loader.getVertices();
	normals = loader.getNormals();
	indices = loader.getVertexIndices();

	// Create and compile our GLSL program from the shaders
	program = LoadShaders("toon.vs", "toon.fs");
	
	
	// Use our shader
	glUseProgram(program);
	view = glm::lookAt(vec3(0.0f, 0.0f, 2.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
	projection = mat4(1.0f);

	// Initialize shader lighting parameters
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	unsigned int vbo[3];
	glGenBuffers(3, vbo);
	
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	
	glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
	
	glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glEnableVertexAttribArray(0);  // Vertex position
	
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);  // Vertex normal
	
	glBindVertexArray(0);

	vec4 light_position(10.0f, 10.0f, 10.0f, 1.0f);


	
	glUniform4fv(glGetUniformLocation(program, "LightPosition"), 1, (GLfloat*)&light_position);

	
	
	glClearColor(1.0, 1.0, 1.0, 1.0);

}
void Display(void)
{
	// Clear
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
    glUseProgram(program);
	// Choose whether to draw in wireframe mode or not
	model = mat4(1.0f);

	mat4 scaled = glm::scale(glm::mat4(1.0f), glm::vec3(gCameraScale, gCameraScale, gCameraScale));
	mat4 translated = glm::translate(glm::mat4(1.0f), glm::vec3(gCameraTranslationX, gCameraTranslationY, 0.0));

	mat4 transformation_matrix = translated*gCameraRotation*scaled;

	model *= transformation_matrix;
	
	model_view = view*model;

	mat3 normalmatrix = mat3(vec3(model_view[0]), vec3(model_view[1]), vec3(model_view[2]));

	
	projection = glm::perspective(70.0f, aspect, 0.3f, 100.0f); 
	mat4 mvp = projection*model_view;

	glUniformMatrix4fv(glGetUniformLocation(program, "ModelViewMatrix"), 1, GL_FALSE, (GLfloat*)&model_view[0]);
	glUniformMatrix3fv(glGetUniformLocation(program, "NormalMatrix"), 1, GL_FALSE, (GLfloat*)&normalmatrix[0]);
	glUniformMatrix4fv(glGetUniformLocation(program, "MVP"), 1, GL_FALSE, (GLfloat*)&mvp[0]);
	glUniformMatrix4fv(glGetUniformLocation(program, "ProjectionMatrix"), 1, GL_FALSE, (GLfloat*)&projection[0]);

	glBindVertexArray(vao);
	
	//glBindTexture(GL_TEXTURE_2D, bunnyTexID);
	glDrawArrays(GL_TRIANGLES, 0, vertices.size());
	glBindVertexArray(0);
	
	glutSwapBuffers();
}

/*******************************************************************************************************************/
void Reshape(int width, int height)
{
	width = glutGet(GLUT_WINDOW_WIDTH);
	height = glutGet(GLUT_WINDOW_HEIGHT);

	gViewportWidth = width;
	gViewportHeight = height;
	glViewport(0, 0, width, height);

	aspect = float(width) / float(height);
}

/******************************************************************************************************************************/

void keyboard(unsigned char key, int x, int y){
	switch (key){
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

/*********************************************************************************************/
void makeIdentity(){

	gCameraScale = 1.0;
	gCameraTranslationX = 0;
	gCameraTranslationY = 0;
	gCameraRotation = mat4(1.0f);


}
/************************************************************************************************/
/*******************************************************************************/
double projectToTrackball(double radius, double x, double y)
{
	static const double kUnitSphereRadius2D = sqrt(2.0);
	double z;

	double dist = sqrt(x * x + y * y);
	if (dist < radius * kUnitSphereRadius2D / 2.0)
	{
		// Solve for sphere case.
		z = sqrt(radius * radius - dist * dist);
	}
	else
	{
		// Solve for hyperbolic sheet case.
		double t = radius / kUnitSphereRadius2D;
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
			gIsScalingCamera = true;
		}
		else if (button == GLUT_RIGHT_BUTTON)
		{
			gIsTranslatingCamera = true;
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

		lastPos.x = gLastMouseX * 2.0 / gViewportWidth - 1.0;
		lastPos.y = (gViewportHeight - gLastMouseY) * 2.0 / gViewportHeight - 1.0;
		lastPos.z = projectToTrackball(kTrackBallRadius, lastPos[0], lastPos[1]);

		currPos.x = x * 2.0 / gViewportWidth - 1.0;
		currPos.y = (gViewportHeight - y) * 2.0 / gViewportHeight - 1.0;
		currPos.z = projectToTrackball(kTrackBallRadius, currPos[0], currPos[1]);
		
		currPos = normalize(currPos);
		lastPos = normalize(lastPos);

		vec3 rotateVecCross;

		rotateVecCross = cross(vec3(lastPos), vec3(currPos)) ;
		
		double sin_rotate_angle = sqrt(rotateVecCross.x * rotateVecCross.x + rotateVecCross.y * rotateVecCross.y + rotateVecCross.z * rotateVecCross.z);
		double angle = asin(sqrt(rotateVecCross.x * rotateVecCross.x + rotateVecCross.y * rotateVecCross.y + rotateVecCross.z * rotateVecCross.z));
		double cos_rotate_angle = cos(angle);
		
		rotateVecCross = normalize(rotateVecCross);

		mat4 deltaRotation(1.0f);

		deltaRotation[0][0] = cos_rotate_angle + rotateVecCross[0] * rotateVecCross[0] * (1 - cos_rotate_angle);
		deltaRotation[0][1] = rotateVecCross[0] * rotateVecCross[1] * (1 - cos_rotate_angle) + rotateVecCross[2] * sin_rotate_angle;
		deltaRotation[0][2] = rotateVecCross[0] * rotateVecCross[2] * (1 - cos_rotate_angle) - rotateVecCross[1] * sin_rotate_angle;
		deltaRotation[0][3] = 0.0;
		deltaRotation[1][0] = rotateVecCross[0] * rotateVecCross[1] * (1 - cos_rotate_angle) - rotateVecCross[2] * sin_rotate_angle;
		deltaRotation[1][1] = cos_rotate_angle + rotateVecCross[1] * rotateVecCross[1] * (1 - cos_rotate_angle);
		deltaRotation[1][2] = rotateVecCross[2] * rotateVecCross[1] * (1 - cos_rotate_angle) + rotateVecCross[0] * sin_rotate_angle;
		deltaRotation[1][3] = 0.0;
		deltaRotation[2][0] = rotateVecCross[0] * rotateVecCross[2] * (1 - cos_rotate_angle) + rotateVecCross[1] * sin_rotate_angle;
		deltaRotation[2][1] = rotateVecCross[1] * rotateVecCross[2] * (1 - cos_rotate_angle) - rotateVecCross[0] * sin_rotate_angle;
		deltaRotation[2][2] = cos_rotate_angle + rotateVecCross[2] * rotateVecCross[2] * (1 - cos_rotate_angle);
		

		mat4 tempMatrix;

		
		if (abs(angle) > DBL_EPSILON*20.0){
			tempMatrix = gCameraRotation*deltaRotation;
			gCameraRotation = tempMatrix;
		}
	}
	if (gIsTranslatingCamera)
	{
		gCameraTranslationX += 2 * double(x - gLastMouseX) / gViewportWidth;
		gCameraTranslationY -= 2 * double(y - gLastMouseY) / gViewportHeight;

	}
	else if (gIsScalingCamera)
	{
		float y1 = gViewportHeight - gLastMouseY;
		float y2 = gViewportHeight - y;

		gCameraScale *= 1 + (y1 - y2) / gViewportHeight;
	}

	glutPostRedisplay();

	gLastMouseX = x;
	gLastMouseY = y;
}
/************************************************************************************************************************************************************/

/*******************************************************************************
*******************************************************************************/

int main(int argc, char** argv){

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA);
	glutInitWindowSize(512, 512);

	glutCreateWindow("Toon shaded bunny");

	if (glewInit()){
		std::cerr << "Unable to initialize GLEW ... exiting" << std::endl;
	}
	
	Initialize();
	std::cout << glGetString(GL_VERSION) << std::endl;
	glutDisplayFunc(Display);
	glutKeyboardFunc(keyboard);
	glutMotionFunc(glutMotion);
	glutMouseFunc(glutMouse);
	glutReshapeFunc(Reshape);
	glutMainLoop();
	return 0;
}


