#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>


using namespace std;

float aspect;
GLuint vao[1];
GLuint vbo[1];
GLuint ebo[1];

GLuint v, f;

static const GLchar*
ReadFile(const char* filename)
{
	FILE* infile;

#ifdef WIN32

	fopen_s(&infile, filename, "r");

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

GLuint initShaders() {

	GLuint p = glCreateProgram();
	v = glCreateShader(GL_VERTEX_SHADER);
	f = glCreateShader(GL_FRAGMENT_SHADER);

	const char * vs = ReadFile("drawCommands.VERT");
	const char * fs = ReadFile("drawCommands.FRAG");

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


void init(void){


	// Set up the vertex attributes
	glGenVertexArrays(1, vao);
	glBindVertexArray(vao[0]);


	GLuint program = initShaders();


	static const GLfloat vertex_positions[] =
	{

		-0.45f, -0.45f, 0.0f, 1.0f,
		0.45f, -0.45f, 0.0f, 1.0f,
		0.45f, 0.45f, 0.0f, 1.0f,

		-0.45f, 0.45f, 0.0f, 1.0f,
		-0.45f, -0.45f, 0.0f, 1.0f,


	};

	// Color for each vertex
	static const GLfloat vertex_colors[] =
	{
		0.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 1.0f, 1.0f,

	};

	static const GLushort vertex_indices[] =
	{
		0, 1, 2,
		2, 3, 0

	};

	glGenBuffers(1, vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_positions) + sizeof(vertex_colors), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertex_positions), vertex_positions);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertex_positions), sizeof(vertex_colors), vertex_colors);


	// Set up the element array buffer

	glGenBuffers(1, ebo);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(vertex_indices), vertex_indices, GL_STATIC_DRAW);


	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, NULL);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (const GLvoid *)sizeof(vertex_positions));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void display(void){

	glClear(GL_COLOR_BUFFER_BIT);

	glBindVertexArray(vao[0]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[0]);

	glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_SHORT, NULL);
	//glDrawElementsBaseVertex(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, NULL, 0); // set base to '1'

	glFlush();
}

/***/
int main(int argc, char** argv){

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA);
	glutInitWindowSize(1024, 768);

	glutCreateWindow(argv[0]);
	glewExperimental = GL_TRUE;
	if (glewInit()){
		cerr << "Unable to initialize GLEW ... exiting" << endl;
	}

	init();
	std::cout << glGetString(GL_VERSION) << std::endl;
	glutDisplayFunc(display);
	glutMainLoop();
}

/*****/

