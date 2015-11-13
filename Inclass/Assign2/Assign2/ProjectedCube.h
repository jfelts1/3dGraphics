//header to remove clutter from ProjectedCube.cpp
#ifndef PROJECTEDCUBE_H
#define PROJECTEDCUBE_H
#ifdef _MSC_VER
#if _MSC_VER < 1900
#pragma message("Untested on this compiler, may not work")
#endif
#pragma warning(disable: 4505 4514 4668 4820 4710 4711 4265)
#endif
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>
#include <functional>
#include <cmath>
#include <utility>

#define GLM_FORCE_RADIANS 
#define GLM_FORCE_CXX11

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp> 
#include "Utils/ShaderUtils.h"
#include "cubedata.h"

#define SCREEN_SIZE_X 512
#define SCREEN_SIZE_Y 512

void init();
void display(void);
void Reshape(int width, int height);
void keyboard(unsigned char key, int x, int y);
void initAttributeIndices();
void initUniformIndices();
void initCubeLocs();

GLuint cube_vao;
GLuint cube_vbo;
GLuint cube_ebo;
GLuint loc;
GLuint normal_loc;
GLuint matrix_loc;
GLuint projection_matrix_loc;
GLuint view_matrix_loc;
GLuint program;

static const double kPI = 3.1415926535897932384626433832795;

glm::mat4 view_matrix;
glm::mat4 projection_matrix;

float aspect = 0.0;

glm::vec3 eye(0.0f, 0.0f, 2.0f);
glm::vec3 center(0.0f);

GLfloat zNear = 1.5f;
GLfloat zFar = 10.0f;

bool cull = false;
bool line = false;
bool orthographic = false;
bool isometric = false;
bool dimetric = false;
bool trimetric = false;
bool onePoint = false;
bool twoPoint = false;
bool threePoint = false;
#endif

