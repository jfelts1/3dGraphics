#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cmath>

#define GLM_FORCE_RADIANS 
#define GLM_FORCE_CXX11
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective


GLuint  cube_vao;
GLuint  cube_vbo;
GLuint  cube_ebo;
GLuint loc;
GLuint normal_loc;
GLuint color_loc;
GLuint matrix_loc;

GLuint program;
static const double kPI = 3.1415926535897932384626433832795;
using namespace glm;


static const GLfloat cube_vertices[] = { 1.0f, 1.0f, 1.0f, 1.0f,  // v0,v1,v2,v3 (front)
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
1.0f, 1.0f, -1.0f, 1.0f };

static const GLfloat cube_normals[] = { 0.0f, 0.0f, 1.0f, // v0,v1,v2,v3 (front)
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
0.0f, 0.0f, -1.0f };


static const GLushort cube_indices[] = { 0, 1, 2, 2, 3, 0,            // front
4, 5, 6, 6, 7, 4,            // right
8, 9, 10, 10, 11, 8,         // top
12, 13, 14, 14, 15, 12,      // left
16, 17, 18, 18, 19, 16,      // bottom
20, 21, 22, 22, 23, 20 };    // back

static const GLfloat colors[] = { 1.0f, 1.0f, 1.0f, 1.0f,
1.0f, 0.0f, 0.0f, 1.0f,
0.0f, 1.0f, 0.0f, 1.0f,
0.0f, 0.0f, 1.0f, 1.0f,
1.0f, 1.0f, 0.0f, 1.0f };

static const GLfloat positions[] = { 0.0f, 0.0f, 0.0f, 1.0f,
-3.0f, -3.0f, 0.0f, 1.0f,
3.0f, -3.0f, 0.0f, 1.0f,
3.0f, 3.0f, 0.0f, 1.0f,
-3.0f, 3.0f, 0.0f, 1.0f };



static const GLchar* ReadFile(const char* filename);
GLuint initShaders(const char* v_shader, const char* f_shader);
void init();
void display(void);

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

	auto source = new GLchar[len + 1];
	fread(source, 1, len, infile);
	fclose(infile);
	source[len] = 0;

	return const_cast<const GLchar*> (source);
}

GLuint initShaders(const char *v_shader, const char *f_shader) 
{
	auto p = glCreateProgram();
	auto v = glCreateShader(GL_VERTEX_SHADER);
	auto f = glCreateShader(GL_FRAGMENT_SHADER);

	auto vs = ReadFile(v_shader);
	auto fs = ReadFile(f_shader);

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

		GLchar* log = new GLchar[len + 1];
		glGetShaderInfoLog(v, len, &len, log);
		std::cerr << "Vertex Shader compilation failed: " << log << std::endl;
		delete[] log;


	}

	glCompileShader(f);

	glGetShaderiv(f, GL_COMPILE_STATUS, &compiled);
	if (!compiled) {
		GLsizei len;
		glGetShaderiv(f, GL_INFO_LOG_LENGTH, &len);

		GLchar* log = new GLchar[len + 1];
		glGetShaderInfoLog(f, len, &len, log);
		std::cerr << "Vertex Shader compilation failed: " << log << std::endl;
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

		GLchar* log = new GLchar[len + 1];
		glGetProgramInfoLog(p, len, &len, log);
		std::cerr << "Shader linking failed: " << log << std::endl;
		delete[] log;
	}
	glUseProgram(p);
	return p;

}

void Initialize(void)
{
	// Create the program for rendering the model

	program = initShaders("cube_shader.vs", "cube_shader.fs");


	// attribute indices

	loc = glGetAttribLocation(program, "position");
	normal_loc = glGetAttribLocation(program, "normal");

	// uniform indices

	matrix_loc = glGetUniformLocation(program, "model_matrix");
	color_loc = glGetUniformLocation(program, "color");


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

	glBindVertexArray(cube_vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cube_ebo);

	// Draw the center cube

	glm::mat4 model_matrix;
	glUniformMatrix4fv(matrix_loc, 1, GL_FALSE, reinterpret_cast<GLfloat*>(&model_matrix[0]));
	glUniform4fv(color_loc, 1, const_cast<GLfloat*>(&colors[0]));
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, nullptr);


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
	//glewExperimental = GL_TRUE;
	if (glewInit()){
		std::cerr << "Unable to initialize GLEW ... exiting" << std::endl;
	}

	Initialize();
	std::cout << glGetString(GL_VERSION) << std::endl;
	glutDisplayFunc(Display);

	glutMainLoop();
	return 0;
}




/*************/



