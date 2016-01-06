#include <GL/glew.h>
#include <GL/freeglut.h>

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <vector>
#include <map>
#include <set>

#define GLM_FORCE_RADIANS 
#define TWOPI 2*3.1415926535897932384626433832795

#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective


#include "shader.h"

using namespace glm;
using namespace std;

mat4 view;
mat4 model;
mat4 projection;
mat4 model_view;

GLuint programOne;
float aspect = 0.0;
GLfloat angle = 0.0;
int viewport[4];

GLuint vao_ps;
GLuint vbo_ps;



#define POINT_COUNT 40

void Initialize();
void display(void);
void makeIdentity();
void Reshape(int width, int height);
void glutMouse(int button, int state, int x, int y);
void glutMotion(int x, int y);
double projectToTrackball(double radius, double x, double y);

static inline float random_float()
{
	float res;
	unsigned int tmp;
	static unsigned int seed = 0x13371337;

	seed *= 16807;

	tmp = seed ^ (seed >> 4) ^ (seed << 15);

	*((unsigned int *)&res) = (tmp >> 9) | 0x3F800000;

	return (res - 1.0f);
}

void Initialize(void){
	// Create the program for rendering the model

	
	// Read our .obj file
	
	// Use our shader
	
	view = glm::lookAt(vec3(0.0f, 0.0f, 2.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
	projection = mat4(1.0f);

	
	
	programOne = LoadShaders("points.vs.glsl", "points.fs.glsl");

	
	glUseProgram(programOne);

	glEnable(GL_POINT_SPRITE);
	
	static vec4 * vertex_positions;

	glGenVertexArrays(1, &vao_ps);
	glBindVertexArray(vao_ps);

	glGenBuffers(1, &vbo_ps);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_ps);
	glBufferData(GL_ARRAY_BUFFER, POINT_COUNT * sizeof(vec4), NULL, GL_STATIC_DRAW);

	vertex_positions = (vec4 *)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
	for (int n = 0; n < POINT_COUNT; n++)
	{
		vertex_positions[n] = vec4(random_float() * 2.0f - 1.0f, random_float() * 2.0f - 1.0f, random_float() * 2.0f - 1.0f, 1.0f);

	}

	 
	glUnmapBuffer(GL_ARRAY_BUFFER);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);
	
	
	glClearColor(1.0, 1.0, 1.0, 1.0);

}
void Display(void)
{
	// Clear
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glGetIntegerv(GL_VIEWPORT, viewport);
    glUseProgram(programOne);
	// Choose whether to draw in wireframe mode or not
	model = mat4(1.0f);

	projection = glm::perspective(70.0f, aspect, 0.3f, 100.0f); 
	
	glUseProgram(programOne);
	//float t = float(GetTickCount() & 0x1FFF) / float(0x1FFF);
	static float q = 0.0f;
	static const vec3 X(1.0f, 0.0f, 0.0f);
	static const vec3 Y(0.0f, 1.0f, 0.0f);
	static const vec3 Z(0.0f, 0.0f, 1.0f);
	
	mat4 projection_matrix = glm::perspective(70.0f, aspect, 0.3f, 100.0f); //frustum(-1.0f, 1.0f, -aspect, aspect, 1.0f, 8.0f);
	
	glEnable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);  // for point sprites
	
	glEnable(GL_PROGRAM_POINT_SIZE);
	glPointSize(32.0f);

	glUniformMatrix4fv(glGetUniformLocation(programOne, "projection_matrix"), 1, GL_FALSE, (GLfloat*)&projection_matrix[0]);
	mat4 model_matrix = translate(mat4(1.0f), vec3(0.0f, 0.0f, -2.0f)) *rotate(mat4(1.0f),radians(/*t **/ 360.0f),Y) * rotate(mat4(1.0f), radians(/*t **/ 720.0f), Z);
	
	glUniformMatrix4fv(glGetUniformLocation(programOne, "model_matrix"), 1, GL_FALSE, (GLfloat*)&model_matrix[0]);
	
	
	glBindVertexArray(vao_ps);

	
	glDrawArrays(GL_POINTS, 0, POINT_COUNT);

	glBindVertexArray(0);

	
	glutSwapBuffers();
}



void Reshape(int width, int height)
{
	width = glutGet(GLUT_WINDOW_WIDTH);
	height = glutGet(GLUT_WINDOW_HEIGHT);

	
	glViewport(0, 0, width, height);

	//projection = glm::perspective(50.0f, (float)width / height, 0.3f, 1000.0f);
	aspect = float(width) / float(height);
}


void keyboard(unsigned char key, int x, int y){
	switch (key){
	case 'q':case 'Q':
		exit(EXIT_SUCCESS);
		break;

	}
	glutPostRedisplay();
}

/*********************************************************************************************/

/************************************************************************************************/


/*******************************************************************************
*******************************************************************************/

/*********/
int main(int argc, char** argv)
{
    glewExperimental = GL_TRUE;
    glutInitContextVersion(3,3);
    
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA);
	glutInitWindowSize(512, 512);

	glutCreateWindow("Points");

	if (glewInit()){
		std::cerr << "Unable to initialize GLEW ... exiting" << std::endl;
	}
	Initialize();
	std::cout << glGetString(GL_VERSION) << std::endl;
	glutDisplayFunc(Display);
	glutKeyboardFunc(keyboard);
	glutReshapeFunc(Reshape);
	glutMainLoop();
	return 0;
}


