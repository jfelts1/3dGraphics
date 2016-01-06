//#pragma once
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
static_assert(NumPointsPerTriangle == 3, "Triangles must have 3 points.");

GLuint  cone_vao;
GLuint  cone_vbo;
GLuint  cone_ebo;

GLuint vertexPosition;
GLuint vertexNormal;
GLuint light_loc;
GLuint material_loc;

glm::mat4 view;
glm::mat4 projection;
glm::mat3 normalMat;
glm::mat4 modelViewMat;

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

glm::vec3 computeVertexNormal(const std::vector<glm::vec3> normalsToAvg);
std::vector<glm::vec3> getAdjacentTriangleNormals(const size_t ind);
void printIndices();

template<class InputIt,class T>
bool contains(InputIt first,InputIt last, const T& value)
{
	auto res = std::find(first, last, value);
	if(res != last)
	{
		return true;
	}
	return false;
}
//#include "SmoothCone.h"
using namespace glm;
using std::vector;
using std::array;
using std::advance;
using std::copy;

void initializeCone() {

	//top point of cone
	points[index][0] = 0.0;
	points[index][1] = 1.0;
	points[index][2] = 0.0;
	points[index][3] = 1.0;

	normals[index][0] = 0.0;
	normals[index][1] = 0.0;
	normals[index][2] = 0.0;

	index++;

	float theta;
	size_t tIndices = 0;

	for (size_t i = 0; i < NumConePoints; ++i, index++)
	{
		theta = static_cast<float>(i*20.0f*kPI / 180.0f);

		points[index][0] = cos(theta);
		points[index][1] = -1.0;
		points[index][2] = -sin(theta);
		points[index][3] = 1.0;

		normals[index][0] = 0.0;
		normals[index][1] = 0.0;
		normals[index][2] = 0.0;

		if (i <= (NumConePoints - 2))
		{
			indices[tIndices] = 0u;
			tIndices++;
			indices[tIndices] = index;
			tIndices++;
			indices[tIndices] = index + 1;
			tIndices++;
		}
		//last triangle
		else
		{
			indices[tIndices] = 0u;
			tIndices++;
			indices[tIndices] = index;
			tIndices++;
			indices[tIndices] = 1u;
			tIndices++;
		}
	}
	printIndices();
	updateVertexNormals();

}

void updateVertexNormals()
{
	for (size_t i = 0;i < points.size();i++)
	{
        vector<vec3> normalsVec = getAdjacentTriangleNormals(i);
        normals[i] = computeVertexNormal(normalsVec);
        //printf("%llu: %f,%f,%f\n",i,normals[i][0],normals[i][1],normals[i][2]);
        normalsVec.clear();
	}
}

vec3 computeVertexNormal(const vector<vec3> normalsVec)
{
	vec3 ret;
    for(auto& norm:normalsVec)
	{
		ret += norm;
	}
    return normalize(ret);
}

vector<vec3> getAdjacentTriangleNormals(const size_t ind)
{
	vector<vec3> ret;
	static array<GLuint, NumPointsPerTriangle> tmp;

	for (auto beg = indices.begin(), end = indices.end();beg != end;advance(beg,NumPointsPerTriangle))
	{
		copy(beg, beg+NumPointsPerTriangle, tmp.begin());
		if (contains(tmp.begin(), tmp.end(), ind))
		{
            ret.emplace_back(triangleNormal(vec3(points[tmp[0]]), vec3(points[tmp[1]]), vec3(points[tmp[2]])));
		}
	}
	return ret;
}

void printIndices()
{
    for (size_t i = 0;i < indices.size();i+=NumPointsPerTriangle)
	{
		printf("i1:%i, i2:%i, i3:%i\n", indices[i], indices[i + 1], indices[i + 2]);
	}
}

static const GLchar* ReadFile(const char* filename)
{
	FILE* infile;

#ifdef WIN32

	fopen_s(&infile, filename, "rb");

#else
	infile = fopen(filename, "r");
#endif
	if (!infile) {
		std::cerr << "Unable to open file '" << filename << "'" << std::endl;
		return nullptr;
	}

	fseek(infile, 0, SEEK_END);
	int len = ftell(infile);
	fseek(infile, 0, SEEK_SET);

	GLchar* source = new GLchar[static_cast<size_t>(len + 1)];
	fread(source, 1, static_cast<size_t>(len), infile);
	fclose(infile);
	source[len] = 0;

	return const_cast<const GLchar*> (source);
}

GLuint initShaders(const char *v_shader, const char *f_shader) {

	GLuint p = glCreateProgram();
	GLuint v = glCreateShader(GL_VERTEX_SHADER);
	GLuint f = glCreateShader(GL_FRAGMENT_SHADER);

	const char * vs = ReadFile(v_shader);
	const char * fs = ReadFile(f_shader);

	glShaderSource(v, 1, &vs, nullptr);
	glShaderSource(f, 1, &fs, nullptr);
	delete[] vs;
	delete[] fs;

	glCompileShader(v);
	GLint compiled;
	glGetShaderiv(v, GL_COMPILE_STATUS, &compiled);
	if (!compiled) {
		GLsizei len;
		glGetShaderiv(v, GL_INFO_LOG_LENGTH, &len);

		GLchar* log = new GLchar[static_cast<size_t>(len + 1)];
		glGetShaderInfoLog(v, len, &len, log);
		std::cerr << "Vertex Shader compilation failed: " << log << std::endl;
		delete[] log;


	}

	glCompileShader(f);

	glGetShaderiv(f, GL_COMPILE_STATUS, &compiled);
	if (!compiled) {
		GLsizei len;
		glGetShaderiv(f, GL_INFO_LOG_LENGTH, &len);

		GLchar* log = new GLchar[static_cast<size_t>(len + 1)];
		glGetShaderInfoLog(f, len, &len, log);
		std::cerr << "Fragment Shader compilation failed: " << log << std::endl;
		delete[] log;
	}

	glAttachShader(p, v);
	glAttachShader(p, f);
	glLinkProgram(p);

	GLint linked;
	glGetProgramiv(p, GL_LINK_STATUS, &linked);
	if (!linked) {
		GLsizei len;
		glGetProgramiv(p, GL_INFO_LOG_LENGTH, &len);

		GLchar* log = new GLchar[static_cast<size_t>(len + 1)];
		glGetProgramInfoLog(p, len, &len, log);
		std::cerr << "Shader linking failed: " << log << std::endl;
		delete[] log;
	}
	glUseProgram(p);
	return p;
}

void Display(void)
{
    // Clear
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Choose whether to draw in wireframe mode or not

    if (show_line)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    else
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    vec4 lightpos = view*vec4(0.0f, 0.0f, 2.0f, 1.0f);
    glUniform4fv(glGetUniformLocation(program, "Light.Position"), 1, reinterpret_cast<GLfloat*>(&lightpos));


    // Setup matrices

    mat4 model = translate(mat4(1.0f), vec3(0.0f, 0.0f, -1.0f));
    projection = perspective(70.0f, aspect, 0.3f, 100.0f);
    mat4 mvp = projection*view*model;
    modelViewMat = model*view;
    normalMat = mat3(modelViewMat);


    glUniformMatrix4fv(glGetUniformLocation(program, "MVP"), 1, GL_FALSE, reinterpret_cast<GLfloat*>(&mvp[0]));
    glUniformMatrix4fv(glGetUniformLocation(
        program, "ProjectionMatrix"), 1, GL_FALSE, reinterpret_cast<GLfloat*>(&projection[0]));

    // You need to add normal matrix and model view matrix
    glUniformMatrix3fv(glGetUniformLocation(
                           program, "NormalMatrix"), 1, GL_FALSE, reinterpret_cast<GLfloat*>(&normalMat[0]));
    glUniformMatrix4fv(glGetUniformLocation(
                           program, "ModelViewMatrix"), 1, GL_FALSE, reinterpret_cast<GLfloat*>(&modelViewMat[0]));

    glBindVertexArray(cone_vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cone_ebo);


    glDrawElements(GL_TRIANGLES, NumIndices, GL_UNSIGNED_INT, nullptr);
    glutSwapBuffers();
}

void Initialize(void){
	// Create the program for rendering the model

	initializeCone();

	GLuint offset = 0;
	glGenVertexArrays(1, &cone_vao);
	glBindVertexArray(cone_vao);
	glGenBuffers(1, &cone_vbo);

	glBindBuffer(GL_ARRAY_BUFFER, cone_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points) + sizeof(normals), nullptr, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, offset, sizeof(points), points.data());
	offset += sizeof(points);
    glBufferSubData(GL_ARRAY_BUFFER, offset, sizeof(normals), normals.data());

	glGenBuffers(1, &cone_ebo);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cone_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices.data(), GL_STATIC_DRAW);

	program = initShaders("smoothshader.vert", "smoothshader.frag");

	// attribute indices

    vertexPosition = static_cast<GLuint>(glGetAttribLocation(program, "VertexPosition"));
    glVertexAttribPointer(vertexPosition, 4, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(vertexPosition);

    vertexNormal = static_cast<GLuint>(glGetAttribLocation(program, "VertexNormal"));
    glVertexAttribPointer(vertexNormal, 3, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<GLvoid *>(sizeof(points)));
    glEnableVertexAttribArray(vertexNormal);
	
	view = lookAt(vec3(0.0f, 0.0f, 5.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
	
	// Initialize shader material and lighting parameters

	vec3 material_ambient(0.9, 0.5, 0.3);
	vec3 material_diffuse(0.9, 0.5, 0.3);
	vec3 material_specular(0.8, 0.8, 0.8);
	
	vec3 light_ambient(0.4, 0.4, 0.4);
	vec3 light_diffuse(1.0, 1.0, 1.0);
	vec3 light_specular(1.0, 1.0, 1.0);

	float material_shininess = 150.0f;

	glUniform3fv(glGetUniformLocation(program, "Material.Ka"), 1, reinterpret_cast<GLfloat*>(&material_ambient));
	glUniform3fv(glGetUniformLocation(program, "Light.La"), 1, reinterpret_cast<GLfloat*>(&light_ambient));
	
	// Set other lighting parameters here

	glUniform3fv(glGetUniformLocation(program, "Material.Kd"), 1, reinterpret_cast<GLfloat*>(&material_diffuse));
	glUniform3fv(glGetUniformLocation(program, "Light.Ld"), 1, reinterpret_cast<GLfloat*>(&light_diffuse));
	glUniform3fv(glGetUniformLocation(program, "Material.Ks"), 1, reinterpret_cast<GLfloat*>(&material_specular));
	glUniform3fv(glGetUniformLocation(program, "Light.Ls"), 1, reinterpret_cast<GLfloat*>(&light_specular));
    glUniform1f(glGetUniformLocation(program, "Material.Shininess"), material_shininess);

	
	projection = mat4(1.0f);
	
	
	glEnable(GL_DEPTH_TEST);

    glClearColor(0.0, 0.0, 0.0, 1.0);

}

void Reshape(int width, int height)
{
	glViewport(0, 0, width, height);

	aspect = float(width) / float(height);
}


// ReSharper disable CppParameterNeverUsed
void keyboard(unsigned char key, int x, int y){
	// ReSharper restore CppParameterNeverUsed
	switch (key){
	case 'q':case 'Q':
		exit(EXIT_SUCCESS);
		break;

	case 's':
		show_line = !show_line;
		break;
	}
	glutPostRedisplay();
}

/*********/
int main(int argc, char** argv){

    glewExperimental = GL_TRUE;
    glutInitContextVersion(3,3);

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA);
	glutInitWindowSize(512, 512);

	glutCreateWindow("SmoothCone");

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

/*************/

