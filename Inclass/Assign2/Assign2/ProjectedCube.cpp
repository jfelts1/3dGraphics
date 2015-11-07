#ifdef _MSC_VER
#if _MSC_VER < 1900
#pragma message("Untested on this compiler, may not work")
#endif
#pragma warning(disable: 4505 4514 4668 4820 4710 4711)
#endif
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>
#include <cstdlib>
#include <functional>
#include <cmath>

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

GLuint cube_vao;
GLuint cube_vbo;
GLuint cube_ebo;
GLuint loc;
GLuint normal_loc;
GLuint matrix_loc;
GLuint projection_matrix_loc;
GLuint view_matrix_loc;
GLuint program;

//different boolean variables

bool orthographic = true;
bool show_line = false;
bool cullface = false;

static const double kPI = 3.1415926535897932384626433832795;

glm::mat4 view_matrix;
glm::mat4 projection_matrix;
using namespace glm;
using std::function;

float aspect = 0.0;

vec3 eye(0.0f,0.0f,2.0f);
vec3 center(0.0f);
//GLfloat center[3] = { 0.0f, 0.0f, 0.0f };


GLfloat zNear = 1.5f;
GLfloat zFar = 10.0f;

// Choose whether the back face will be enabled or not
auto cullToggle = Toggle<function<void()>,function<void()>,void>(
[]()
{
    std::cout<<"Cull enabled"<<std::endl;
    glEnable(GL_CULL_FACE);
}
,
[]()
{
    std::cout<<"Cull disabled"<<std::endl;
    glDisable(GL_CULL_FACE);
});

// Choose whether to draw in wireframe mode or not
auto lineToggle = Toggle<function<void()>,function<void()>,void>(
[]()
{
    std::cout<<"Line enabled"<<std::endl;
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}
,
[]()
{
    std::cout<<"Line disabled"<<std::endl;
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
});

// Choose the type of Projection
auto orthographicToggle = Toggle<function<glm::mat4(GLfloat,GLfloat)>,function<glm::mat4 (GLfloat,GLfloat)>,glm::mat4>(
[&](GLfloat zNear, GLfloat zFar)
{
    std::cout<<"Orthographic disabled"<<std::endl;
    return frustum(-1.0f, 1.0f, -1.0f, 1.0f, zNear, zFar);
}
,
[&](GLfloat zNear, GLfloat zFar)
{
    std::cout<<"Orthographic enabled"<<std::endl;
    return ortho(-1.0f, 1.0f, -1.0f, 1.0f, zNear, zFar);
});

auto isometricToggle = Toggle<function<glm::mat4(vec3,vec3)>,function<glm::mat4(vec3,vec3)>,glm::mat4>(
[&](vec3 eye,vec3 center)
{
    std::cout<<"Isometric enabled"<<std::endl;
   // mat3 tmp(\
             sqrt(3.0f),0.0f,-sqrt(3.0f),\
             1.0f,2.0f,1.0f,\
             sqrt(2.0f),-sqrt(2.0f),sqrt(2.0f));
   // tmp*=(1.0f/sqrt(6.0f));
   // vec3 eyeTmp = tmp*eye;
    return glm::lookAt(eye,center,vec3(1.0f));
}
,
[&](vec3 eye, vec3 center)
{
    std::cout<<"Isometric disabled"<<std::endl;
    return glm::lookAt(eye,center,vec3(-1.0f));
});

void Initialize(void)
{
	// Create the program for rendering the model

	program = Utils::initShaders("shader_projected.vs", "shader_projected.fs");

	// attribute indices

	loc = static_cast<GLuint>(glGetAttribLocation(program, "position"));
	normal_loc = static_cast<GLuint>(glGetAttribLocation(program, "normal"));
	projection_matrix_loc = static_cast<GLuint>(glGetUniformLocation(program, "projection_matrix"));
	
	// uniform indices
	
	view_matrix_loc = static_cast<GLuint>(glGetUniformLocation(program, "view_matrix"));
	matrix_loc = static_cast<GLuint>(glGetUniformLocation(program, "model_matrix"));


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

    //init draw
    projection_matrix = orthographicToggle.toggle(zNear,zFar);
    lineToggle.toggle();
    cullToggle.toggle();
    projection_matrix = isometricToggle.toggle(eye,center);
}


void Display(void)
{
	// Clear
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Setup

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);


	// Setup view matrix

    view_matrix = lookAt(eye, center, vec3(0.0f, 1.0f, 0.0f));

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
void keyboard(unsigned char key, int x, int y){

	switch (key)
	{
	case 'q':case 'Q':
		exit(EXIT_SUCCESS);
		break;

	case 'o':
        projection_matrix = orthographicToggle.toggle(zNear,zFar);
		break;
	case 's':
        lineToggle.toggle();
		break;
	case 'c':
        cullToggle.toggle();
		break;
	
	case 'i':
		// implement isometric view
        projection_matrix = isometricToggle.toggle(eye,center);
        for(int i =0;i<3;i++)
        {
            for(int j =0;j<3;j++)
            {
                printf("%f ",projection_matrix[i][j]);
            }
            printf("\n");
        }
		break;
	case 'd':
		//implement dimetric view
		break;
	case 't':
		// implement trimetric view
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
		std::cerr << "Unable to initialize GLEW ... exiting" << std::endl;
	}

	Initialize();
	std::cout << glGetString(GL_VERSION) << std::endl;
	glutDisplayFunc(Display);
	glutKeyboardFunc(keyboard);
	glutReshapeFunc(Reshape);
	glutMainLoop();
	return EXIT_SUCCESS;
}




/*************/



