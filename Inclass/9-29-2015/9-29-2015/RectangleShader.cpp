#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

GLuint  rect_vao;
GLuint  rect_vbo;
GLuint v, f, p;

static const GLchar* ReadFile(const char* filename);
void initShaders();
void init();
void display(void);

static const GLchar*
ReadFile(const char* filename)
{

	FILE* infile;
	fopen_s(&infile, filename, "rb");

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

void initShaders() {

	p = glCreateProgram();
	v = glCreateShader(GL_VERTEX_SHADER);
	f = glCreateShader(GL_FRAGMENT_SHADER);

	const char * vs = ReadFile("rectangle.vs");
	const char * fs = ReadFile("rectangle.fs");

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

}

void init(){

	GLfloat rect_vertices[] = {
		-0.90f, -0.90f,
		0.90f, -0.90f,
		0.90f, 0.90f,
		-0.90f, 0.90f,
	};

	glGenVertexArrays(1, &rect_vao);
	glBindVertexArray(rect_vao);
	glGenBuffers(1, &rect_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, rect_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(rect_vertices), rect_vertices, GL_STATIC_DRAW);


	initShaders();

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
	glClearColor(1.0, 1.0, 1.0, 1.0);
}



void display(void){

	glClear(GL_COLOR_BUFFER_BIT);
	glBindVertexArray(rect_vao);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	glFlush();
}

/***/
int main(int argc, char** argv){

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