#pragma once
#ifdef _MSC_VER
#if _MSC_VER < 1900
#pragma message("Untested on this compiler, may not work")
#endif
#pragma warning(disable: 4505 4514 4668 4820 4710 4711)
#endif
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <array>
#include <vector>
#include <algorithm>
#include <iterator>

#define GLM_FORCE_RADIANS 
#define GLM_FORCE_CXX11

#include <glm/glm.hpp>
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtx/normal.hpp>

#define NumConePoints  18
#define NumTriangles   18
#define NumPointsPerTriangle 3
#define NumIndices NumPointsPerTriangle*NumTriangles
typedef std::array<GLuint,NumPointsPerTriangle> triangleIndices;
static_assert(NumPointsPerTriangle == 3, "Triangles must have 3 points.");

GLuint  cone_vao;
GLuint  cone_vbo;
GLuint  cone_ebo;

GLuint position_loc;
GLuint normal_loc;
GLuint light_loc;
GLuint material_loc;

glm::mat4 view;
glm::mat4 projection;

GLuint program;

float aspect = 0.0;
bool show_line = false;

std::array<GLuint,NumIndices> indices;
std::array<glm::vec4, NumConePoints + 1> points;
std::array<glm::vec3, NumConePoints + 1> normals;

static const double kPI = 3.1415926535897932384626433832795;
GLuint index = 0;

static const GLchar* ReadFile(const char* filename);
GLuint initShaders(const char* v_shader, const char* f_shader);
void init();
void display(void);
void Reshape(int width, int height);
void initializeCone();
void updateVertexNormals();

glm::vec3 computeVertexNormal(const std::vector<glm::vec3> normalsToAvg) noexcept;
std::vector<glm::vec3> getAdjacentTriangleNormals(const size_t ind) noexcept;
void printIndices() noexcept;

template<class InputIt,class T>
bool contains(InputIt first,InputIt last, const T& value) noexcept
{
	auto res = std::find(first, last, value);
	if(res != last)
	{
		return true;
	}
	return false;
}