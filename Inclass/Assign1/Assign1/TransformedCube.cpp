#ifdef _MSC_VER
#if _MSC_VER < 1900
#pragma message("Untested on this compiler, may not work")
#endif
#pragma warning(disable: 4505 4514 4668 4820 4710 4711)
#endif
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <array>
#include <vector>

#define GLM_FORCE_RADIANS 
#define GLM_FORCE_CXX11
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective

#include "Utils/FileUtils.h"
#include "Utils/ShaderUtils.h"
#include "Cube.h"

#define NUM_CUBES 5

constexpr double kPI = 3.1415926535897932384626433832795;
using namespace glm;
std::array<Cube, NUM_CUBES> cubes;

static const std::vector<GLfloat> verts = {
	1.0f, 1.0f, 1.0f, 1.0f,  // v0,v1,v2,v3 (front)
	-1.0f, 1.0f, 1.0f, 1.0f,
	-1.0f, -1.0f, 1.0f, 1.0f,
	1.0f, -1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f, 1.0f,  // v0,v3,v4,v5 (right)
	1.0f, -1.0f, 1.0f, 1.0f,
	1.0f, -1.0f, -1.0f, 1.0f,
	1.0f, 1.0f, -1.0f, 1.0f,
	1.0f, 1.0f, 1.0f, 1.0f,  // v0,v5,v6,v1 (top)
	1.0f, 1.0f, -1.0, 1.0f,
	-1.0f, 1.0f, -1.0f, 1.0f,
	-1.0f, 1.0f, 1.0f, 1.0f,
	-1.0f, 1.0f, 1.0f, 1.0f, // v1,v6,v7,v2 (left)
	-1.0f, 1.0f, -1.0f, 1.0f,
	-1.0f, -1.0f, -1.0f, 1.0f,
	-1.0f, -1.0f, 1.0f, 1.0f,
	-1.0f, -1.0f, -1.0f, 1.0f,// v7,v4,v3,v2 (bottom)
	1.0f, -1.0f, -1.0f, 1.0f,
	1.0f, -1.0f, 1.0f, 1.0f,
	-1.0f, -1.0f, 1.0f, 1.0f,
	1.0f, -1.0f, -1.0f, 1.0f,// v4,v7,v6,v5 (back)
	-1.0f, -1.0f, -1.0f, 1.0f,
	-1.0f, 1.0f, -1.0f, 1.0f,
	1.0f, 1.0f, -1.0f, 1.0f
};

static const std::vector<GLfloat> norms = {
	0.0f, 0.0f, 1.0f, // v0,v1,v2,v3 (front)
	0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 0.0f, // v0,v3,v4,v5 (right)
	1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f, // v0,v5,v6,v1 (top)
	0.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	-1.0f, 0.0f, 0.0f,// v1,v6,v7,v2 (left)
	-1.0f, 0.0f, 0.0f,
	-1.0f, 0.0f, 0.0f,
	-1.0f, 0.0f, 0.0f,
	0.0f, -1.0f, 0.0f, // v7,v4,v3,v2 (bottom)
	0.0f, -1.0f, 0.0f,
	0.0f, -1.0f, 0.0f,
	0.0f, -1.0f, 0.0f,
	0.0f, 0.0f, -1.0f, // v4,v7,v6,v5 (back)
	0.0f, 0.0f, -1.0f,
	0.0f, 0.0f, -1.0f,
	0.0f, 0.0f, -1.0f
};

static const std::vector<GLushort> indices = { 
0, 1, 2, 2, 3, 0,            // front
4, 5, 6, 6, 7, 4,            // right
8, 9, 10, 10, 11, 8,         // top
12, 13, 14, 14, 15, 12,      // left
16, 17, 18, 18, 19, 16,      // bottom
20, 21, 22, 22, 23, 20		 // back
};   

static const std::array<std::array<GLfloat, COLOR_ARRAY_SIZE>, NUM_CUBES> colors = {
	std::array<GLfloat,COLOR_ARRAY_SIZE>{1.0f, 1.0f, 1.0f, 1.0f},
	std::array<GLfloat,COLOR_ARRAY_SIZE>{1.0f, 0.0f, 0.0f, 1.0f},
	std::array<GLfloat,COLOR_ARRAY_SIZE>{0.0f, 1.0f, 0.0f, 1.0f},
	std::array<GLfloat,COLOR_ARRAY_SIZE>{0.0f, 0.0f, 1.0f, 1.0f},
	std::array<GLfloat,COLOR_ARRAY_SIZE>{1.0f, 1.0f, 0.0f, 1.0f}
};

static const std::array<std::array<GLfloat, POSITION_ARRAY_SIZE>, NUM_CUBES> pos = {
	std::array<GLfloat,POSITION_ARRAY_SIZE>{0.0f, 0.0f, 0.0f, 1.0f},
	std::array<GLfloat,POSITION_ARRAY_SIZE>{-3.0f, -3.0f, 0.0f, 1.0f},
	std::array<GLfloat,POSITION_ARRAY_SIZE>{3.0f, -3.0f, 0.0f, 1.0f},
	std::array<GLfloat,POSITION_ARRAY_SIZE>{3.0f, 3.0f, 0.0f, 1.0f},
	std::array<GLfloat,POSITION_ARRAY_SIZE>{-3.0f, 3.0f, 0.0f, 1.0f},
};

void Initialize(void)
{
	Cube::initCubeShaders();
	cubes[0] = Cube(verts, norms, indices, colors[0], pos[0]);
	//cubes[1] = Cube(verts, norms, indices, colors[1], pos[1]);

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
}

void Display(void)
{
	// Clear
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Setup
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	// Setup
	cubes[0].display();
	//cubes[1].display();


	// Draw cube translated

	// You'll draw the red cube here
	// position is defined in positions array
	// you need to change the model_view matrix
	// you also need to change the color, change the color array index




	// Draw cube scaled

	// You'll draw the green cube here
	// position is defined in positions array
	// you need to change the model_view matrix
	// you also need to change the color, change the color array index



	// Draw cube rotated

	// You'll draw the blue cube here
	// position is defined in positions array
	// you need to change the model_view matrix
	// you also need to change the color, change the color array index


	// Draw the cube rotated about an angle of 45 degree about an arbitrary axis (1.0f, 2.0f, 3.0f)

	// You'll draw the yellow cube here
	// position is defined in positions array
	// you need to change the model_view matrix
	// you also need to change the color, change the color array index


	glFlush();
}

/*********/
int main(int argc, char** argv)
{
	glewExperimental = GL_TRUE;
	glutInitContextVersion (3, 3);
	
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA);
	glutInitWindowSize(512, 512);

	glutCreateWindow(argv[0]);

	if (glewInit())
	{
		std::cerr << "Unable to initialize GLEW ... exiting" << std::endl;
	}

	Initialize();
	std::cout << glGetString(GL_VERSION) << std::endl;
	glutDisplayFunc(Display);

	glutMainLoop();
	return 0;
}




/*************/



