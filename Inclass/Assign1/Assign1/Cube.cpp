#include "Cube.h"

Cube::Cube()
{
}

Cube::~Cube()
{
}

GLuint Cube::m_cubeProgram = 0;
GLuint Cube::m_cubeLoc = 0;
GLuint Cube::m_cubeNormalLoc = 0;
GLuint Cube::m_cubeMatrixLoc = 0;
GLuint Cube::m_cubeColorLoc = 0;

GLuint Cube::cube_vao = 0;
GLuint Cube::cube_vbo = 0;
GLuint Cube::cube_ebo = 0;
