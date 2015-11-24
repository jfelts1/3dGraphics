//Adapted from Angel's Book

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define GLM_FORCE_RADIANS 

#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective


GLuint  cube_vao;
GLuint  cube_vbo;

GLuint position_loc;
GLuint normal_loc;

GLuint projection_matrix_loc;
GLuint model_view_matrix_loc;
GLuint program;

using namespace glm;

float aspect = 0.0;
bool show_line = false;


const int NumTimesToSubdivide = 5;
const int NumTriangles = 4096; // (4 faces)^(NumTimesToSubdidvide + 1)
const int NumVertices = 3 * NumTriangles;

vec4 points[NumVertices];
vec4 colors;

vec3 normals[NumVertices];

int index = 0;

void triangle(const vec4 &p1, const vec4 &p2, const vec4 &p3){

	vec3 normal = normalize(cross(vec3(p2 - p1), vec3(p3 - p1)));


	normals[index] = normal; points[index] = p1; index++;
	normals[index] = normal; points[index] = p2; index++;
	normals[index] = normal; points[index] = p3; index++;

}



vec4 unit(const vec4& p){
	float len = p.x*p.x + p.y*p.y + p.z*p.z;

	vec4 t;

	if (len > 0.0){

		t = p / sqrt(len);
		t.w = 1.0;
	}
	
	return t;
}

void divide_triangle(const vec4 &p1, const vec4 &p2, const vec4 &p3, int count){

	if (count > 0){

		
		vec4 v1 = unit(p1 + p2);
		vec4 v2 = unit(p1 + p3);
		vec4 v3 = unit(p2 + p3);
		

		divide_triangle(p1, v1, v2, count - 1);
		divide_triangle(p3, v2, v3, count - 1);
		divide_triangle(p2, v3, v1, count - 1);
		divide_triangle(v1, v3, v2, count - 1);

		
	}

	else{
		triangle(p1, p2, p3);
	}
}

void tetrahedron(int count){

	vec4 point[4] = { { 0.0, 0.0, 1.0, 1.0 },
	{ 0.0, 0.942809, -0.333333, 1.0 },
	{ -0.816497, -0.471405, -0.333333, 1.0 },
	{ 0.816497, -0.471405, -0.333333, 1.0 } };

	divide_triangle(point[0], point[1], point[2], count);
	divide_triangle(point[3], point[2], point[1], count);
	divide_triangle(point[0], point[3], point[1], count);
	divide_triangle(point[0], point[2], point[3], count);

}

static const GLchar* ReadFile(const char* filename);
GLuint initShaders(const char* v_shader, const char* f_shader);
void init();
void display(void);
void Reshape(int width, int height);

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
	GLuint v = glCreateShader(GL_VERTEX_SHADER);
	GLuint f = glCreateShader(GL_FRAGMENT_SHADER);

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

		GLchar* log = new GLchar[len + 1];
		glGetProgramInfoLog(p, len, &len, log);
		std::cerr << "Shader linking failed: " << log << std::endl;
		delete[] log;
	}
	glUseProgram(p);
	return p;

}

void Initialize(void){
	// Create the program for rendering the model


	tetrahedron(NumTimesToSubdivide);

	GLuint offset = 0;
	glGenVertexArrays(1, &cube_vao);
	glBindVertexArray(cube_vao);
	glGenBuffers(1, &cube_vbo);

	glBindBuffer(GL_ARRAY_BUFFER, cube_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points) + sizeof(normals), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, offset, sizeof(points), points);
	offset += sizeof(points);
	glBufferSubData(GL_ARRAY_BUFFER, offset, sizeof(normals), normals);

	program = initShaders("subroutine_shader.vs", "subroutine_shader.fs");

	// attribute indices

	position_loc = glGetAttribLocation(program, "position");
	glVertexAttribPointer(position_loc, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(position_loc);

	normal_loc = glGetAttribLocation(program, "normal");
	glVertexAttribPointer(normal_loc, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid *)sizeof(points));
	glEnableVertexAttribArray(normal_loc);

	// Initialize shader lighting parameters

	vec4 light_position(0.0, 0.0, 2.0, 0.0);  // directional light source
	vec4 light_ambient(0.2, 0.2, 0.2, 1.0);
	vec4 light_diffuse(1.0, 1.0, 1.0, 1.0);
	vec4 light_specular(1.0, 1.0, 1.0, 1.0);


	vec4 material_ambient(1.0, 0.0, 1.0, 1.0);
	vec4 material_diffuse(1.0, 0.8, 0.0, 1.0);
	vec4 material_specular(1.0, 0.0, 1.0, 1.0);
	float material_shininess = 5.0;

	vec4 ambient_product = light_ambient*material_ambient;
	vec4 diffuse_product = light_diffuse*material_diffuse;
	vec4 specular_product = light_specular*material_specular;

	glUniform4fv(glGetUniformLocation(program, "AmbientProduct"), 1, (GLfloat*)&ambient_product[0]);
	glUniform4fv(glGetUniformLocation(program, "DiffuseProduct"), 1, (GLfloat*)&diffuse_product[0]);
	glUniform4fv(glGetUniformLocation(program, "SpecularProduct"), 1, (GLfloat*)&specular_product[0]);

	glUniform4fv(glGetUniformLocation(program, "LightPosition"), 1, (GLfloat*)&light_position[0]);
	glUniform1f(glGetUniformLocation(program, "Shininess"), material_shininess);

	// Retrieve transformation uniform variable locations

	projection_matrix_loc = glGetUniformLocation(program, "projection");
	model_view_matrix_loc = glGetUniformLocation(program, "model_view");

	glEnable(GL_DEPTH_TEST);

	glClearColor(0.0, 0.0, 0.0, 1.0);

}

void Display(void)
{
	// Clear
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Choose whether to draw in wireframe mode or not

	if (show_line)
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	
	// Setup view matrix

	vec4 eye(0.0, 0.0, 2.0, 1.0);
	vec4 center(0.0, 0.0, 0.0, 1.0);


	mat4 model_matrix = lookAt(vec3(eye[0], eye[1], eye[2]), vec3(center[0], center[1], center[2]), vec3(0.0f, 1.0f, 0.0f));

	glUniformMatrix4fv(model_view_matrix_loc, 1, GL_FALSE, (GLfloat*)&model_matrix[0]);
	GLuint adsIndex = glGetSubroutineIndex(program, GL_VERTEX_SHADER, "phongModel");
	GLuint diffuseIndex = glGetSubroutineIndex(program, GL_VERTEX_SHADER, "diffuseOnly");

	glUniformSubroutinesuiv(GL_VERTEX_SHADER, 1, &diffuseIndex);
	//glUniformSubroutinesuiv(GL_VERTEX_SHADER, 1, &adsIndex);

	// Choose the type of Projection

	mat4 projection_matrix;

	if (aspect >= 1.0)
		projection_matrix = glm::ortho(-2.0f*aspect, 2.0f*aspect, -2.0f, 2.0f, -20.0f, 20.0f);
	else
		projection_matrix = glm::ortho(-2.0f, 2.0f, -2.0f / aspect, 2.0f / aspect, -20.0f, 20.0f);



	glUniformMatrix4fv(projection_matrix_loc, 1, GL_FALSE, (GLfloat*)&projection_matrix[0]);



	glBindVertexArray(cube_vao);


	glDrawArrays(GL_TRIANGLES, 0, NumVertices);

	glutSwapBuffers();
}

void Reshape(int width, int height)
{
	glViewport(0, 0, width, height);

	aspect = float(width) / float(height);
}


void keyboard(unsigned char key, int x, int y){
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

	glutCreateWindow("Sphere");

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
