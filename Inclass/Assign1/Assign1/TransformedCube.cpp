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

constexpr float kPI = 3.1415926535897932384626433832795f;
using std::array;
array<Cube, NUM_CUBES> cubes;

static const array<array<GLfloat, COLOR_ARRAY_SIZE>, NUM_CUBES> colors = {
	array<GLfloat,COLOR_ARRAY_SIZE>{1.0f, 1.0f, 1.0f, 1.0f},
	array<GLfloat,COLOR_ARRAY_SIZE>{1.0f, 0.0f, 0.0f, 1.0f},
	array<GLfloat,COLOR_ARRAY_SIZE>{0.0f, 1.0f, 0.0f, 1.0f},
	array<GLfloat,COLOR_ARRAY_SIZE>{0.0f, 0.0f, 1.0f, 1.0f},
	array<GLfloat,COLOR_ARRAY_SIZE>{1.0f, 1.0f, 0.0f, 1.0f}
};

static const array<array<GLfloat, POSITION_ARRAY_SIZE>, NUM_CUBES> pos = {
	array<GLfloat,POSITION_ARRAY_SIZE>{0.0f, 0.0f, 0.0f, 1.0f},
	array<GLfloat,POSITION_ARRAY_SIZE>{-3.0f, -3.0f, 0.0f, 1.0f},
	array<GLfloat,POSITION_ARRAY_SIZE>{3.0f, -3.0f, 0.0f, 1.0f},
	array<GLfloat,POSITION_ARRAY_SIZE>{3.0f, 3.0f, 0.0f, 1.0f},
	array<GLfloat,POSITION_ARRAY_SIZE>{-3.0f, 3.0f, 0.0f, 1.0f},
};

void Initialize(void)
{
	Cube::initCubeShaders();
	for (size_t i = 0;i < NUM_CUBES;i++)
	{	
		cubes[i] = Cube(colors[i], pos[i]);
	}

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

	Cube::bind();

	cubes[2].setScale(glm::vec3(1.5f, 1.5f, 1.0f));
	cubes[3].setRotate(glm::vec3(1.0f,0.0f,0.0f),45.0f*(kPI)/180.0f);
	for(auto& cu:cubes)
	{
		cu.display();
	}


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



