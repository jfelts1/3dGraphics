#ifndef SHADERUTILS_H
#define SHADERUTILS_H
#ifdef _MSC_VER
#pragma warning(disable: 4505 4514 4668 4820 4710 4711)
#endif
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>
#include "FileUtils.h"

namespace Utils
{
	void CompileShader(GLuint shader);
	void LinkShader(GLuint shader);
	GLuint initShaders(const char *v_shader, const char *f_shader);
	
}


#endif

