#ifdef _MSC_VER
#if _MSC_VER < 1900
#pragma message("Untested on this compiler, may not work")
#endif
#pragma warning(disable: 4505 4514 4668 4820 4710 4711 4265)
#endif
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>
#include <cstdlib>
#include <functional>
#include <cmath>
#include <utility>

#define GLM_FORCE_RADIANS 
#define GLM_FORCE_CXX11

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp> 
#include "Utils/ShaderUtils.h"
#include "Toggle.h"
#include "cubedata.h"

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
using namespace glm;
using std::function;
using std::pair;
using std::cout;
using std::cin;
using std::endl;

float aspect = 0.0;

vec3 eye(0.0f,0.0f,2.0f);
vec3 center(0.0f);

GLfloat zNear = 1.5f;
GLfloat zFar = 10.0f;

// Choose whether the back face will be enabled or not
Toggle<function<void()>, function<void()>, void> cullToggle(
[]()
{
    cout<<"Cull enabled"<<endl;
    glEnable(GL_CULL_FACE);
}
,
[]()
{
    cout<<"Cull disabled"<<endl;
    glDisable(GL_CULL_FACE);
});

// Choose whether to draw in wireframe mode or not
Toggle<function<void()>, function<void()>, void> lineToggle(
[]()
{
    cout<<"Line enabled"<<endl;
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}
,
[]()
{
    cout<<"Line disabled"<<endl;
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
});

// Choose the type of Projection
Toggle<function<pair<mat4,mat4>(GLfloat, GLfloat)>, function<pair<mat4,mat4>(GLfloat, GLfloat)>, pair<mat4,mat4>> orthographicToggle(
[&](GLfloat Near, GLfloat Far)
{
    cout<<"Orthographic disabled"<<endl;
    return pair<mat4,mat4>(lookAt(eye, center, vec3(0.0f, 1.0f, 0.0f)),frustum(-1.0f, 1.0f, -1.0f, 1.0f, Near, Far));
}
,
[&](GLfloat Near, GLfloat Far)
{
    cout<<"Orthographic enabled"<<endl;
    return pair<mat4,mat4>(lookAt(eye, center, vec3(0.0f, 1.0f, 0.0f)),ortho(-1.0f, 1.0f, -1.0f, 1.0f, Near, Far));
});

Toggle<function<pair<mat4, mat4>(vec3, vec3)>, function<pair<mat4, mat4>(vec3, vec3)>, pair<mat4, mat4>> isometricToggle(
[&](vec3 Eye,vec3 Center)
{
   cout<<"Isometric enabled"<<endl;
   mat3 tmp(
             sqrt(3.0f),0.0f,-sqrt(3.0f),
             1.0f,2.0f,1.0f,
             sqrt(2.0f),-sqrt(2.0f),sqrt(2.0f));
   tmp*=(1.0f/sqrt(6.0f));
   vec3 eyeTmp = tmp*Eye;
   return pair<mat4,mat4>(lookAt(eyeTmp,Center,vec3(-1.0f,-1.0f,-1.0f)), ortho(2.0f, -2.0f, -2.0f, 2.0f, -5.0f, 5.0f));
}
,
[&](vec3 Eye, vec3 Center)
{
    cout<<"Isometric disabled"<<endl;
	return pair<mat4, mat4>(lookAt(Eye, Center, vec3(0.0f, 1.0f, 0.0f)), ortho(-1.0f, 1.0f, -1.0f, 1.0f, 1.5f, 10.0f));
});

Toggle<function<pair<mat4, mat4>(vec3, vec3)>, function<pair<mat4, mat4>(vec3, vec3)>, pair<mat4, mat4>> dimetricToggle(
	[&](vec3 Eye, vec3 Center)
{
	cout << "Dimetric enabled" << endl;
	const float phi = radians(15.0f);
	const float theta = radians(45.0f);
	mat3 xMat(
		1.0f, 0.0f, 0.0f,
		0.0f, cos(phi), -sin(phi),
		0.0f, sin(phi), cos(phi));
	mat3 yMat(
		cos(theta),0.0f,sin(theta),
		0.0f,1.0f,0.0f,
		-sin(theta),0.0f,cos(theta));
	vec3 eyeTmp = Eye*xMat*yMat;
	return pair<mat4, mat4>(lookAt(eyeTmp, Center, vec3(-1.0f, -1.0f, -1.0f)), ortho(2.0f, -2.0f, -2.0f, 2.0f, -5.0f, 5.0f));
}
,
[&](vec3 Eye, vec3 Center)
{
	cout << "Dimetric disabled" << endl;
	return pair<mat4, mat4>(lookAt(Eye, Center, vec3(0.0f, 1.0f, 0.0f)), ortho(-1.0f, 1.0f, -1.0f, 1.0f, 1.5f, 10.0f));
});

Toggle<function<pair<mat4, mat4>(vec3, vec3)>, function<pair<mat4, mat4>(vec3, vec3)>, pair<mat4, mat4>> trimetricToggle(
	[&](vec3 Eye, vec3 Center)
{
	cout << "Trimetric enabled" << endl;
	const float gamma = radians(5.0f);
	const float beta = radians(30.0f);
	const float psi = radians(0.0f);

	mat3 xMat(
		1.0f, 0.0f, 0.0f,
		0.0f, cos(gamma), -sin(gamma),
		0.0f, sin(gamma), cos(gamma));
	mat3 yMat(
		cos(beta), 0.0f, sin(beta),
		0.0f, 1.0f, 0.0f,
		-sin(beta), 0.0f, cos(beta));
	mat3 zMat(
		cos(psi),sin(psi),0.0f,
		-sin(psi),cos(psi),0.0f,
		0.0f,0.0f,1.0f);
	vec3 eyeTmp = Eye*xMat*yMat*zMat;
	return pair<mat4, mat4>(lookAt(eyeTmp, Center, vec3(-1.0f, -1.0f, -1.0f)), ortho(2.0f, -2.0f, -2.0f, 2.0f, -5.0f, 5.0f));
}
,
[&](vec3 Eye, vec3 Center)
{
	cout << "Trimetric disabled" << endl;
	return pair<mat4, mat4>(lookAt(Eye, Center, vec3(0.0f, 1.0f, 0.0f)), ortho(-1.0f, 1.0f, -1.0f, 1.0f, 1.5f, 10.0f));
});

void Initialize(void)
{
	initCubeLocs();

	GLuint offset = 0;
	glGenVertexArrays(1, &cube_vao);
	glBindVertexArray(cube_vao);
	glGenBuffers(1, &cube_vbo);

	glBindBuffer(GL_ARRAY_BUFFER, cube_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices) + sizeof(cube_normals), nullptr, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, offset, sizeof(cube_vertices), cube_vertices);
	offset += sizeof(cube_vertices);
	glBufferSubData(GL_ARRAY_BUFFER, offset, sizeof(cube_normals), cube_normals);


	glGenBuffers(1, &cube_ebo);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cube_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);

	glVertexAttribPointer(loc, 4, GL_FLOAT, GL_FALSE, 0, nullptr);
	glEnableVertexAttribArray(loc);

	glVertexAttribPointer(normal_loc, 3, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<GLvoid *>(sizeof(cube_vertices)));
	glEnableVertexAttribArray(normal_loc);

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    //initial draw setup
    auto orthPair = orthographicToggle.toggle(zNear,zFar);
	view_matrix = orthPair.first;
	projection_matrix = orthPair.second;

    lineToggle.toggle();
    cullToggle.toggle();
    auto isoPair = isometricToggle.toggle(eye,center);
	view_matrix = isoPair.first;
	projection_matrix = isoPair.second;

	auto diPair = dimetricToggle.toggle(eye, center);
	view_matrix = diPair.first;
	projection_matrix = diPair.second;

	auto triPair = trimetricToggle.toggle(eye, center);
	view_matrix = triPair.first;
	projection_matrix = triPair.second;
}

void Display(void)
{
	// Clear
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Setup

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);


	// Setup view matrix

    //view_matrix = lookAt(eye, center, vec3(0.0f, 1.0f, 0.0f));

	glUniformMatrix4fv(static_cast<GLint>(view_matrix_loc), 1, GL_FALSE, reinterpret_cast<GLfloat*>(&view_matrix[0]));


	// Choose the type of Projection

    glUniformMatrix4fv(
                static_cast<GLint>(projection_matrix_loc), 1, GL_FALSE, reinterpret_cast<GLfloat*>(&projection_matrix[0]));
	mat4 model_matrix;
	glUniformMatrix4fv(static_cast<GLint>(matrix_loc), 1, GL_FALSE, reinterpret_cast<GLfloat*>(&model_matrix[0]));


	glBindVertexArray(cube_vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cube_ebo);

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, nullptr);

	glutSwapBuffers();
}

void Reshape(int width, int height)
{
	glViewport(0, 0, width, height);

	aspect = float(width) / float(height);
}

void printMatrix(mat4& mat)
{
	for(int i =0;i<4;i++)
	{
		for(int j =0;j<4;j++)
		{
			printf("%f ",mat[i][j]);
		}
		printf("\n");
	}
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'q':case 'Q':
		exit(EXIT_SUCCESS);
		break;

	case 'o':
		{
			auto orthPair = orthographicToggle.toggle(zNear, zFar);
			view_matrix = orthPair.first;
			projection_matrix = orthPair.second;
		}
		break;
	case 's':
        lineToggle.toggle();
		break;
	case 'c':
        cullToggle.toggle();
		break;
	
	case 'i':
		// implement isometric view
		{
			auto tmp = isometricToggle.toggle(eye, center);
			view_matrix = tmp.first;
			projection_matrix = tmp.second;
		}
		break;
	case 'd':
		//implement dimetric view
		{
			auto diPair = dimetricToggle.toggle(eye, center);
			view_matrix = diPair.first;
			projection_matrix = diPair.second;
		}
		break;
	case 't':
		// implement trimetric view
		{
			auto triPair = trimetricToggle.toggle(eye, center);
			view_matrix = triPair.first;
			projection_matrix = triPair.second;
		}
		break;
	case '1':
		//implement one-point perspective 
		break;
	case '2':
		//implement two-point perspective
		break;
	case '3':
		// implement three point perspective
		break;

	}
	glutPostRedisplay();
}

void initAttributeIndices()
{
	loc = static_cast<GLuint>(glGetAttribLocation(program, "position"));
	normal_loc = static_cast<GLuint>(glGetAttribLocation(program, "normal"));
	projection_matrix_loc = static_cast<GLuint>(glGetUniformLocation(program, "projection_matrix"));
}

void initUniformIndices()
{
	view_matrix_loc = static_cast<GLuint>(glGetUniformLocation(program, "view_matrix"));
	matrix_loc = static_cast<GLuint>(glGetUniformLocation(program, "model_matrix"));
}

void initCubeLocs()
{
	program = Utils::initShaders("shader_projected.vs", "shader_projected.fs");
	initAttributeIndices();
	initUniformIndices();
}

/*********/
int main(int argc, char** argv)
{
    glewExperimental = GL_TRUE;
    glutInitContextVersion(3,3);

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA);
	glutInitWindowSize(512, 512);

	glutCreateWindow("ProjectedCube");

	if (glewInit())
	{
		std::cerr << "Unable to initialize GLEW ... exiting" << endl;
	}

	Initialize();
	cout << glGetString(GL_VERSION) << endl;
	glutDisplayFunc(Display);
	glutKeyboardFunc(keyboard);
	glutReshapeFunc(Reshape);
	glutMainLoop();
	return EXIT_SUCCESS;
}




/*************/



