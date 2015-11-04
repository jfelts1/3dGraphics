#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define GLM_FORCE_RADIANS

#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective

#define NUM_OBJECTS 3

GLuint  vao[NUM_OBJECTS];

GLuint cube_position_loc;
GLuint cube_normal_loc;


GLuint floor_position_loc;
GLuint floor_normal_loc;


GLuint  cube_vbo;
GLuint  cube_ebo;

GLuint  floor_vbo;
GLuint  floor_ebo;
GLuint  floor_vbo2;
GLuint  floor_ebo2;

GLuint matrix_loc;
GLuint projection_matrix_loc;
GLuint view_matrix_loc;


GLuint program;
static const double kPI = 3.1415926535897932384626433832795;
using namespace glm;

float aspect = 0.0;
static const GLfloat cube_vertices[] = { 1.0f, 1.0f, 1.0f, 1.0f,  // v0,v1,v2,v3 (front)
-1.0f, 1.0f, 1.0f, 1.0f,
-1.0f, -1.0f, 1.0f, 1.0f,
1.0f, -1.0f, 1.0f, 1.0f,
1.0f, 1.0f, 1.0f, 1.0f,  // v0,v3,v4,v5 (right)
1.0f, -1.0f, 1.0f, 1.0f,
1.0f, -1.0f, -1.0f, 1.0f,
1.0f, 1.0f, -1.0f, 1.0f,
1.0f, 1.0f, 1.0f, 1.0f,  // v0,v5,v6,v1 (top)
1.0f, 1.0f, -1.0f, 1.0f,
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
1.0f, 0.0f, 0.0f,// v1,v6,v7,v2 (left)
1.0f, 0.0f, 0.0f,
1.0f, 0.0f, 0.0f,
1.0f, 0.0f, 0.0f,
0.0f, 1.0f, 0.0f, // v7,v4,v3,v2 (bottom)
0.0f, 1.0f, 0.0f,
0.0f, 1.0f, 0.0f,
0.0f, 1.0f, 0.0f,
0.0f, 0.0f, 1.0f, // v4,v7,v6,v5 (back)
0.0f, 0.0f, 1.0f,
0.0f, 0.0f, 1.0f,
0.0f, 0.0f, 1.0f
};


static const GLushort cube_indices[] = { 0, 1, 2, 2, 3, 0,            // front
4, 5, 6, 6, 7, 4,            // right
8, 9, 10, 10, 11, 8,         // top
12, 13, 14, 14, 15, 12,      // left
16, 17, 18, 18, 19, 16,      // bottom
20, 21, 22, 22, 23, 20 };    // back

static const GLfloat floor_vertices[] = { -2.0f, -1.0f, -2.0f, 1.0f,// v0,v1,v2,v3 (top)
2.0f, -1.0f, -2.0f, 1.0f,
2.0f, -1.0f, 2.0f, 1.0f,
-2.0f, -1.0f, 2.0f, 1.0f
};

static const GLfloat floor_normals[] = { 0.0f, 1.0f, 0.0f, // v0,v1,v2,v3 (top)
0.0f, 1.0f, 0.0f,
0.0f, 1.0f, 0.0f,
0.0f, 1.0f, 0.0f };

static const GLushort floor_indices[] = { 2, 1, 0, 2, 0, 3 };       // top


static const GLfloat floor_vertices2[] = { -10.0f, 10.0f, -20.0f, 10.0f,// v0,v1,v2,v3 (top)
20.0f, 10.0f, -20.0f, 10.0f,
20.0f, 10.0f, 20.0f, 10.0f,
-20.0f, 10.0f, 20.0f, 10.0f
};

static const GLfloat floor_normals2[] = { 0.0f, 1.0f, 0.0f, // v0,v1,v2,v3 (top)
0.0f, 1.0f, 0.0f,
0.0f, 1.0f, 0.0f,
0.0f, 1.0f, 0.0f };

static const GLushort floor_indices2[] = { 2, 1, 0, 2, 0, 3 };       // top

static const GLchar* ReadFile(const char* filename);
GLuint initShaders(const char* v_shader, const char* f_shader);
void init();
void display(void);
void Reshape(int width, int height);

static const GLchar* ReadFile(const char* filename){
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

void Initialize(void){
    // Create the program for rendering the model

    program = initShaders("multiple_object_shader.vs", "multiple_object_shader.fs");

    // cube attribute indices

    cube_position_loc = glGetAttribLocation(program, "position");
    cube_normal_loc = glGetAttribLocation(program, "normal");

    // floor attribute indices

    floor_position_loc = glGetAttribLocation(program, "position");
    floor_normal_loc = glGetAttribLocation(program, "normal");

    // uniform indices

    projection_matrix_loc = glGetUniformLocation(program, "projection_matrix");
    matrix_loc = glGetUniformLocation(program, "model_matrix");
    view_matrix_loc = glGetUniformLocation(program, "view_matrix");


    // Setup
    glm::mat4 view_matrix = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f));   //isometric
    glUniformMatrix4fv(view_matrix_loc, 1, GL_FALSE, (GLfloat*)&view_matrix[0]);

    glm::mat4 projection_matrix = glm::ortho(-5.0f, 5.0f, -5.0f, 5.0f, 1.0f, 10.0f);

    /*
    if (aspect >= 1.0)
    projection_matrix = glm::ortho(-5.0f*aspect, 5.0f*aspect, -5.0f, 5.0f, 10.0f, -10.0f);
    else
    projection_matrix = glm::ortho(-5.0f, 5.0f, -5.0f/aspect, 5.0f/aspect, 10.0f, -10.0f);
    **/

    glUniformMatrix4fv(projection_matrix_loc, 1, GL_FALSE, (GLfloat*)&projection_matrix[0]);
    glm::mat4 model_matrix;
    glUniformMatrix4fv(matrix_loc, 1, GL_FALSE, (GLfloat*)&model_matrix[0]);

    // initialize cube

    /***/
    GLuint offset_cube = 0;
    glGenVertexArrays(NUM_OBJECTS, vao);

    glBindVertexArray(vao[0]);
    glGenBuffers(1, &cube_vbo);

    glBindBuffer(GL_ARRAY_BUFFER, cube_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices) + sizeof(cube_normals), NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, offset_cube, sizeof(cube_vertices), cube_vertices);
    offset_cube += sizeof(cube_vertices);
    glBufferSubData(GL_ARRAY_BUFFER, offset_cube, sizeof(cube_normals), cube_normals);

    glGenBuffers(1, &cube_ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cube_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);

    glVertexAttribPointer(cube_position_loc, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(cube_position_loc);

    glVertexAttribPointer(cube_normal_loc, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid *)sizeof(cube_vertices));
    glEnableVertexAttribArray(cube_normal_loc);

    glBindVertexArray(0);



    // initialize floor

    GLuint offset_floor = 0;

    glBindVertexArray(vao[1]);
    glGenBuffers(1, &floor_vbo);

    glBindBuffer(GL_ARRAY_BUFFER, floor_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(floor_vertices) + sizeof(floor_normals), NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, offset_floor, sizeof(floor_vertices), floor_vertices);
    offset_floor += sizeof(floor_vertices);
    glBufferSubData(GL_ARRAY_BUFFER, offset_floor, sizeof(floor_normals), floor_normals);

    glGenBuffers(1, &floor_ebo);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, floor_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(floor_indices), floor_indices, GL_STATIC_DRAW);

    glVertexAttribPointer(floor_position_loc, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(floor_position_loc);

    glVertexAttribPointer(floor_normal_loc, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid *)sizeof(floor_vertices));
    glEnableVertexAttribArray(floor_normal_loc);

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);


    GLuint offset_floor2 = 0;

    glBindVertexArray(vao[2]);
    glGenBuffers(1, &floor_vbo2);

    glBindBuffer(GL_ARRAY_BUFFER, floor_vbo2);
    glBufferData(GL_ARRAY_BUFFER, sizeof(floor_vertices2) + sizeof(floor_normals2), NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, offset_floor2, sizeof(floor_vertices2), floor_vertices2);
    offset_floor2 += sizeof(floor_vertices2);
    glBufferSubData(GL_ARRAY_BUFFER, offset_floor2, sizeof(floor_normals2), floor_normals2);

    glGenBuffers(1, &floor_ebo2);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, floor_ebo2);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(floor_indices2), floor_indices2, GL_STATIC_DRAW);

    glVertexAttribPointer(floor_position_loc, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(floor_position_loc);

    glVertexAttribPointer(floor_normal_loc, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid *)sizeof(floor_vertices2));
    glEnableVertexAttribArray(floor_normal_loc);

}


void Display(void)
{
    // Clear
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glEnable(GL_CLIP_DISTANCE0);



    // draws yellow plane

    glBindVertexArray(vao[1]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, floor_ebo);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, NULL);
    glBindVertexArray(0);



    // draws blue box

    glBindVertexArray(vao[0]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cube_ebo);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, NULL);
    glBindVertexArray(0);

    glBindVertexArray(vao[2]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, floor_ebo2);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, NULL);
    glBindVertexArray(0);
    glFlush();
}

void Reshape(int width, int height)
{
    glViewport(0, 0, width, height);

    aspect = float(width) / float(height);
}

/*********/
int main(int argc, char** argv)
{

    glewExperimental = GL_TRUE;
    glutInitContextVersion (3, 3);

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA);
    glutInitWindowSize(512, 512);

    glutCreateWindow("MultipleObjects");

    if (glewInit()){
        std::cerr << "Unable to initialize GLEW ... exiting" << std::endl;
    }

    Initialize();
    std::cout << glGetString(GL_VERSION) << std::endl;
    glutDisplayFunc(Display);
    glutReshapeFunc(Reshape);
    glutMainLoop();
    return 0;
}




/*************/


