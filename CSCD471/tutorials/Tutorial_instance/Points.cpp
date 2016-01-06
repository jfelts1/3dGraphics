#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>


#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective

GLuint  cube_vao;
GLuint  cube_vbo;
GLuint  cube_ebo;
GLuint model_matrix_buffer;
GLuint v, f;
GLuint loc;
GLuint program;
static const double kPI = 3.1415926535897932384626433832795;
using namespace glm;

#define INSTANCE_COUNT 5


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


static const GLushort cube_indices[] = { 0, 1, 2, 2, 3, 0,      // front
4, 5, 6, 6, 7, 4,      // right
8, 9, 10, 10, 11, 8,      // top
12, 13, 14, 14, 15, 12,      // left
16, 17, 18, 18, 19, 16,      // bottom
20, 21, 22, 22, 23, 20 };    // back



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
		return NULL;
	}

	fseek(infile, 0, SEEK_END);
	int len = ftell(infile);
	fseek(infile, 0, SEEK_SET);

	GLchar* source = new GLchar[len + 1];
	fread(source, 1, len, infile);
	fclose(infile);
	source[len] = 0;

	return const_cast<const GLchar*> (source);
}

GLuint initShaders(const char *v_shader, const char *f_shader) {

	GLuint p = glCreateProgram();
	v = glCreateShader(GL_VERTEX_SHADER);
	f = glCreateShader(GL_FRAGMENT_SHADER);

	const char * vs = ReadFile(v_shader);
	const char * fs = ReadFile(f_shader);

	glShaderSource(v, 1, &vs, NULL);
	glShaderSource(f, 1, &fs, NULL);
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
	int position_loc = glGetAttribLocation(program, "position");
	int normal_loc = glGetAttribLocation(program, "normal");
	int color_loc = glGetAttribLocation(program, "instance_color");
	int matrix_loc = glGetAttribLocation(program, "model_matrix");


	static const GLfloat instance_colors[] =
	{
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 0.0f, 1.0f
	};

	static const GLfloat positions[] =
	{
		0.0f, 0.0f, 0.0f, 1.0f,
		-3.0f, -3.0f, 0.0f, 1.0f,
		3.0f, -3.0f, 0.0f, 1.0f,
		3.0f, 3.0f, 0.0f, 1.0f,
		-3.0f, 3.0f, 0.0f, 1.0f
	};

	GLuint offset = 0;
	glGenVertexArrays(1, &cube_vao);
	glBindVertexArray(cube_vao);
	glGenBuffers(1, &cube_vbo);

	glBindBuffer(GL_ARRAY_BUFFER, cube_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices) + sizeof(cube_normals) + sizeof(instance_colors), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, offset, sizeof(cube_vertices), cube_vertices);
	offset += sizeof(cube_vertices);
	glBufferSubData(GL_ARRAY_BUFFER, offset, sizeof(cube_normals), cube_normals);
	offset += sizeof(cube_normals);
	glBufferSubData(GL_ARRAY_BUFFER, offset, sizeof(instance_colors), instance_colors);

	glGenBuffers(1, &cube_ebo);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cube_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);

	glVertexAttribPointer(position_loc, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(position_loc);


	glVertexAttribPointer(normal_loc, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid *)sizeof(cube_vertices));
	glEnableVertexAttribArray(normal_loc);


	glVertexAttribPointer(color_loc, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid *)(sizeof(cube_vertices) + sizeof(cube_normals)));
	glEnableVertexAttribArray(color_loc);
	glVertexAttribDivisor(color_loc, 1);

	glGenBuffers(1, &model_matrix_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, model_matrix_buffer);
	glBufferData(GL_ARRAY_BUFFER, INSTANCE_COUNT * sizeof(mat4), NULL, GL_STATIC_DRAW);

	// Set model matrices for each instance
	glm::mat4 * matrices = (glm::mat4 *)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);

	matrices[0] = glm::mat4(1.0f);
	matrices[1] = glm::translate(glm::mat4(1.0f), glm::vec3(-3.0f, -3.0f, 0.0f));

	mat4 mat2_scaled, mat2_translated;

	mat2_scaled = glm::scale(glm::mat4(1.0f), glm::vec3(1.5f, 1.5f, 1.0));
	mat2_translated = glm::translate(glm::mat4(1.0f), glm::vec3(3.0f, -3.0f, 0.0));

	matrices[2] = mat2_translated*mat2_scaled;
	mat4 mat3_rotated, mat3_translated;

	GLfloat angle = (45.0f * kPI) / 180.0f;

	mat3_rotated = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(1.0f, 0.0f, 0.0f));
	mat3_translated = glm::translate(glm::mat4(1.0f), glm::vec3(3.0f, 3.0f, 0.0f));

	matrices[3] = mat3_translated*mat3_rotated;
	

	GLfloat cos_angle_one = 3.0f / sqrt(13.0f);

	GLfloat angle_one = acos(cos_angle_one);


	GLfloat cos_angle_two = sqrt(13.0f / 14.0f);

	GLfloat angle_two = acos(cos_angle_two);

	mat4 mat4_translated, mat4_rotated_1, mat4_rotated_2, mat4_rotated_angle, mat4_rotated_2_inverse, mat4_rotated_1_inverse;
	mat4_translated = glm::translate(mat4(1.0f), glm::vec3(-3.0f, 3.0f, 0.0f));
	mat4_rotated_1 = glm::rotate(mat4(1.0f), angle_one, glm::vec3(1.0f, 0.0f, 0.0f));
	mat4_rotated_2 = glm::rotate(mat4(1.0f), -angle_two, glm::vec3(0.0f, 1.0f, 0.0f));
	mat4_rotated_angle = glm::rotate(mat4(1.0f), angle, glm::vec3(0.0f, 0.0f, 1.0f));
	mat4_rotated_2_inverse = glm::rotate(mat4(1.0f), angle_two, glm::vec3(0.0f, 1.0f, 0.0f));
	mat4_rotated_1_inverse = glm::rotate(mat4(1.0f), -angle_one, glm::vec3(1.0f, 0.0f, 0.0f));

	matrices[4] = mat4_translated*mat4_rotated_1_inverse*mat4_rotated_2_inverse*mat4_rotated_angle*mat4_rotated_2*mat4_rotated_1;

	glUnmapBuffer(GL_ARRAY_BUFFER);

	// Loop over each column of the matrix...
	for (int i = 0; i < 4; i++)
	{
		// Set up the vertex attribute
		glVertexAttribPointer(matrix_loc + i,              // Location
			4, GL_FLOAT, GL_FALSE,       // vec4
			sizeof(mat4),                // Stride
			(void *)(sizeof(vec4) * i)); // Start offset
		// Enable it
		glEnableVertexAttribArray(matrix_loc + i);
		// Make it instanced
		glVertexAttribDivisor(matrix_loc + i, 1);
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

	// Setup

	glBindVertexArray(cube_vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cube_ebo);
	glDrawElementsInstanced(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, NULL, 5);

	glFlush();
}

/*********/
int main(int argc, char** argv){

    glewExperimental = GL_TRUE;
    glutInitContextVersion(3,3);
    
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA);
	glutInitWindowSize(1024, 1024);

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



