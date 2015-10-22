#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#define GLM_FORCE_RADIANS
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

GLuint  tri_vao;
GLuint  tri_vbo;
GLuint v, f;
GLuint loc;

GLuint matrix_loc;
GLuint color_loc;

static const GLchar* ReadFile(const char* filename);
GLuint initShaders(char* v_shader, char* f_shader);
void init();
void display(void);

static const GLchar*
ReadFile(const char* filename)
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

GLuint initShaders(char* v_shader, char* f_shader) {

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

static const GLfloat tri_vertices[] =
	{
		-1.0f, -1.0f, 0.0f, 1.0f,
		1.0f, -1.0f, 0.0f, 1.0f,
		-1.0f, 1.0f, 0.0f, 1.0f
	};

	static const GLfloat instance_colors[] =
	{
		1.0f, 0.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 0.5f, 1.0f
	};

	static const GLfloat instance_positions[] =
	{
		-2.0f, -2.0f, 0.0f, 1.0f,
		2.0f, -2.0f, 0.0f, 1.0f,
		2.0f, 2.0f, 0.0f, 1.0f,
		-2.0f, 2.0f, 0.0f, 1.0f
	};

void init(){
	GLuint program = initShaders("triangle.vs", "triangle.fs");
	int position_loc = glGetAttribLocation(program, "position");
	matrix_loc = glGetUniformLocation(program,"model_matrix");
	color_loc = glGetUniformLocation(program,"color");	

	GLuint offset = 0;
	glGenVertexArrays(1, &tri_vao);
	glGenBuffers(1, &tri_vbo);
	glBindVertexArray(tri_vao);
	glBindBuffer(GL_ARRAY_BUFFER, tri_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(tri_vertices) + sizeof(instance_colors) + sizeof(instance_positions), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, offset, sizeof(tri_vertices), tri_vertices);
	offset += sizeof(tri_vertices);
	glBufferSubData(GL_ARRAY_BUFFER, offset, sizeof(instance_colors), instance_colors);
	offset += sizeof(instance_colors);
	glBufferSubData(GL_ARRAY_BUFFER, offset, sizeof(instance_positions), instance_positions);
	offset += sizeof(instance_positions);

	glVertexAttribPointer(position_loc, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(position_loc);

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	
}



void display(void){

	glClear(GL_COLOR_BUFFER_BIT);

	glBindVertexArray(tri_vao);
	//glDrawArraysInstanced(GL_TRIANGLES, 0, 3, 4);
	glm::mat4 model_matrix;

	for(int i =0;i<4;i++)
	{
		model_matrix = glm::translate(glm::mat4(1.0f),
		glm::vec3(instance_positions[i*4],instance_positions[(i*4)+1],instance_positions[(i*4)+2]));
		glUniformMatrix4fv(matrix_loc,1,GL_FALSE,(GLfloat*)&model_matrix[0]);
		glUniform4fv(color_loc,1,(GLfloat*)&instance_colors[i]);
		glDrawArrays(GL_TRIANGLES,0,3);
	}
	
	glFlush();
}

/***/
int main(int argc, char** argv){

	glewExperimental = GL_TRUE;
	glutInitContextVersion (3, 3);
	
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA);
	glutInitWindowSize(512, 512);
	glutInitWindowPosition(0, 0);

	glutCreateWindow(argv[0]);

	if (glewInit()){
		std::cerr << "Unable to initialize GLEW ... exiting" << std::endl;
	}

	init();
	std::cout << glGetString(GL_VERSION) << std::endl;
	glutDisplayFunc(display);
	glutMainLoop();
}

/*****/
