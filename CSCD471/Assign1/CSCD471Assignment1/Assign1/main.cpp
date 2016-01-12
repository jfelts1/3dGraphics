#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>
#include <math.h>
#include <vector>

#define GLM_FORCE_RADIANS 
#define GLM_FORCE_CXX11
#define TWOPI 2*3.1415926535897932384626433832795
#define LIGHT_CUTOFF 15.0f

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
GLuint program;
float g_aspect = 0.0;
GLfloat g_angle = 0.0;
GLuint vao;
unsigned int vbo[2];
GLuint ebo;
vector<vec3> vertices;
vector<int> vIndices;
vector<vec3> normals; 
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

	OBJLoader loader;

	bool loadfile = loader.load("bunny.obj");
	if (!loadfile)
	{
		exit(EXIT_FAILURE);
	}
	vertices = loader.getVertices();
	normals = loader.getNormals();
	indices = loader.getVertexIndices();

	// Create and compile our GLSL program from the shaders
	program = LoadShaders("spotlight.vs", "spotlight.fs");
	
	// Use our shader
	glUseProgram(program);
	view = lookAt(vec3(0.0f, 0.0f, 2.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
	projection = mat4(1.0f);

	// Initialize shader lighting parameters
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(2, vbo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(vec3), &vertices[0], GL_DYNAMIC_DRAW);
	glVertexAttribPointer(static_cast<GLuint>(0), 3, GL_FLOAT, GL_FALSE, 0, nullptr);
	glEnableVertexAttribArray(0);  // Vertex position
	
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(vec3), &normals[0], GL_STATIC_DRAW);
	glVertexAttribPointer(static_cast<GLuint>(1), 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);  // Vertex normal

	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(int), &indices[0], GL_STATIC_DRAW);

	glBindVertexArray(0);

	vec3 lightintensity = vec3(0.9f, 0.9f, 0.9f);
	vec3 material_ambient(0.9, 0.5, 0.3);
	vec3 material_diffuse(0.9, 0.5, 0.3);
	vec3 material_specular(0.8, 0.8, 0.8);
	
	glUniform3fv(glGetUniformLocation(program, "Spot.intensity"), 1, reinterpret_cast<GLfloat*>(&lightintensity));
	glUniform3fv(glGetUniformLocation(program, "Ka"), 1, reinterpret_cast<GLfloat*>(&material_ambient));
	glUniform3fv(glGetUniformLocation(program, "Kd"), 1, static_cast<GLfloat*>(&material_diffuse[0]));
	glUniform3fv(glGetUniformLocation(program, "Ks"), 1, static_cast<GLfloat*>(&material_specular[0]));
	glUniform1f(glGetUniformLocation(program, "Shininess"), 180.0f);
	glUniform1f(glGetUniformLocation(program, "Spot.exponent"), 30.0f);
	glUniform1f(glGetUniformLocation(program, "Spot.cutoff"), LIGHT_CUTOFF);

	glClearColor(1.0, 1.0, 1.0, 1.0);
}

void Display(void)
{
	// Clear
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

    glUseProgram(program);
	
	model = mat4(1.0f);
	vec4 lightPos = vec4(10.0f*cos(g_angle), 10.0f, 10.0f*sin(g_angle), 1.0f);
	vec4 spotPos = view*lightPos;
	glUniform4fv(glGetUniformLocation(program, "Spot.position"), 1, reinterpret_cast<GLfloat*>(&spotPos));

	mat4 scaled = scale(mat4(1.0f), vec3(gCameraScale, gCameraScale, gCameraScale));
	mat4 translated = translate(mat4(1.0f), vec3(gCameraTranslationX, gCameraTranslationY, 0.0));
	mat4 transformation_matrix = translated*gCameraRotation*scaled;
	model *= transformation_matrix;
	model_view = view*model;
	mat3 normalmatrix = mat3(view);
	
	// implement spot direction
	vec3 spotDir = normalmatrix*vec3(-spotPos);

	projection = perspective(70.0f, g_aspect, 0.3f, 100.0f); 
	mat4 mvp = projection*model_view;

	glBindVertexArray(vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);


	glUniform3fv(glGetUniformLocation(program, "Spot.direction"),1, static_cast<GLfloat*>(&spotDir[0]));
	glUniformMatrix4fv(glGetUniformLocation(program, "ModelViewMatrix"), 1, GL_FALSE, reinterpret_cast<GLfloat*>(&model_view[0]));
	glUniformMatrix3fv(glGetUniformLocation(program, "NormalMatrix"), 1, GL_FALSE, reinterpret_cast<GLfloat*>(&normalmatrix[0]));
	glUniformMatrix4fv(glGetUniformLocation(program, "MVP"), 1, GL_FALSE, reinterpret_cast<GLfloat*>(&mvp[0]));
	glUniformMatrix4fv(glGetUniformLocation(program, "ProjectionMatrix"), 1, GL_FALSE, reinterpret_cast<GLfloat*>(&projection[0]));

	glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, nullptr);
	
	glBindVertexArray(0);

	glutSwapBuffers();
}

void update(int n)
{
	
	g_angle += 0.1f;
	if (g_angle > static_cast<float>(TWOPI)) g_angle -= static_cast<float>(TWOPI);

	glutPostRedisplay();
	glutTimerFunc(500, update, n);
   
}


// ReSharper disable CppParameterNeverUsed
#ifdef WIN32
#pragma warning(push)
#pragma warning(disable: 4100)
#endif
void keyboard(unsigned char key, int x, int y){
	// ReSharper restore CppParameterNeverUsed
	switch (key)
	{
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
#ifdef WIN32
#pragma warning(pop)
#endif

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
	static const double Radius = sqrt(2.0);
	double z;

	double dist = sqrt(x * x + y * y);
	if (dist < radius * Radius / 2.0)
	{
		// Solve for sphere case.
		z = sqrt(radius * radius - dist * dist);
	}
	else
	{
		// Solve for hyperbolic sheet case.
		double t = radius / Radius;
		z = t * t / dist;
	}

	return z;
}

// ReSharper disable CppParameterValueIsReassigned
void Reshape(int width, int height)
// ReSharper restore CppParameterValueIsReassigned
{
	width = glutGet(GLUT_WINDOW_WIDTH);
	height = glutGet(GLUT_WINDOW_HEIGHT);

	gViewportWidth = width;
	gViewportHeight = height;
	glViewport(0, 0, width, height);

	
	g_aspect = float(width) / float(height);
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
/**************************************************************************************************************************************************************/

void glutMotion(int x, int y)
{
	if (gIsRotatingCamera)
	{
		static const double kTrackBallRadius = 0.8;
		vec3 lastPos;
		vec3 currPos;

		lastPos.x = gLastMouseX * 2.0f / gViewportWidth - 1.0f;
		lastPos.y = (gViewportHeight - gLastMouseY) * 2.0f / gViewportHeight - 1.0f;
		lastPos.z = static_cast<float>(projectToTrackball(kTrackBallRadius, lastPos[0], lastPos[1]));

		currPos.x = x * 2.0f / gViewportWidth - 1.0f;
		currPos.y = (gViewportHeight - y) * 2.0f / gViewportHeight - 1.0f;
		currPos.z = static_cast<float>(projectToTrackball(kTrackBallRadius, currPos[0], currPos[1]));
		
		currPos = normalize(currPos);
		lastPos = normalize(lastPos);

		vec3 rotateVecCross;

		rotateVecCross = cross(vec3(lastPos), vec3(currPos)) ;
		
		double sin_rotate_angle = sqrt(rotateVecCross.x * rotateVecCross.x + rotateVecCross.y * rotateVecCross.y + rotateVecCross.z * rotateVecCross.z);
		double angle = asin(sqrt(rotateVecCross.x * rotateVecCross.x + rotateVecCross.y * rotateVecCross.y + rotateVecCross.z * rotateVecCross.z));
		double cos_rotate_angle = cos(angle);
		
		rotateVecCross = normalize(rotateVecCross);

		mat4 deltaRotation(1.0f);

		deltaRotation[0][0] = static_cast<float>(cos_rotate_angle + rotateVecCross[0] * rotateVecCross[0] * (1 - cos_rotate_angle));
		deltaRotation[0][1] = static_cast<float>(rotateVecCross[0] * rotateVecCross[1] * (1 - cos_rotate_angle) + rotateVecCross[2] * sin_rotate_angle);
		deltaRotation[0][2] = static_cast<float>(rotateVecCross[0] * rotateVecCross[2] * (1 - cos_rotate_angle) - rotateVecCross[1] * sin_rotate_angle);
		deltaRotation[0][3] = 0.0f;
		deltaRotation[1][0] = (float)(rotateVecCross[0] * rotateVecCross[1] * (1 - cos_rotate_angle) - rotateVecCross[2] * sin_rotate_angle);
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
		gCameraTranslationX += 2 * double(x - gLastMouseX) / gViewportWidth;
		gCameraTranslationY -= 2 * double(y - gLastMouseY) / gViewportHeight;

	}
	else if (gIsScalingCamera)
	{
		float y1 = static_cast<float>(gViewportHeight - gLastMouseY);
		float y2 = static_cast<float>(gViewportHeight - y);

		gCameraScale *= 1 + (y1 - y2) / gViewportHeight;
	}

	glutPostRedisplay();

	gLastMouseX = x;
	gLastMouseY = y;
}
/************************************************************************************************************************************************************/
int main(int argc, char** argv)
{
	glewExperimental = GL_TRUE;
    glutInitContextVersion(3,3);
    
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA);
	glutInitWindowSize(512, 512);

	glutCreateWindow("Shiny Gloomy Bunny");

	if (glewInit()){
		cerr << "Unable to initialize GLEW ... exiting" << endl;
	}
	
	Initialize();
	cout << glGetString(GL_VERSION) << endl;
	glutDisplayFunc(Display);
	glutKeyboardFunc(keyboard);
	glutTimerFunc(100, update, 0);
	glutMotionFunc(glutMotion);
	glutMouseFunc(glutMouse);
	glutReshapeFunc(Reshape);
	glutMainLoop();
	return 0;
}


