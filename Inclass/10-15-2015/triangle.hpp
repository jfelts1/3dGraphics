#ifndef TRIANGLE_H
#define TRIANGLE_H
#include <GL/glew.h>
#include <GL/freeglut.h>
#define GLM_FORCE_RADIANS
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <array>

class Triangle
{
public:
	Triangle()=default;

	Triangle(GLuint prog, std::array<GLfloat,4> color, glm::vec4 shift) noexcept
	{		
		m_program = prog;
		m_shift = shift;
		m_positionLoc = glGetAttribLocation(m_program, "position");
		m_colorLoc = glGetAttribLocation(m_program, "instance_color");
		GLuint offset = 0;
		glBufferData(GL_ARRAY_BUFFER, sizeof(tri_vertices)+color.size(), NULL, GL_STATIC_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, offset, sizeof(tri_vertices), tri_vertices);
		offset += sizeof(tri_vertices);
		glBufferSubData(GL_ARRAY_BUFFER, offset, color.size(), color.data());
		
		glVertexAttribPointer(m_positionLoc, 4, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(m_positionLoc);
		
		glVertexAttribPointer(m_colorLoc, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid *)sizeof(tri_vertices));
		glEnableVertexAttribArray(m_colorLoc);
		glVertexAttribDivisor(m_colorLoc, 1);
		
		m_matrixLoc = glGetUniformLocation(m_program,"model_matrix");
		
	}
	
	virtual ~Triangle()noexcept
	{
		
	}
	
	//copy ctor
	Triangle(const Triangle& orig) noexcept : tri_vertices(orig.tri_vertices), m_program(orig.m_program),
											m_positionLoc(orig.m_positionLoc), m_colorLoc(orig.m_colorLoc),
											m_matrixLoc(orig.m_matrixLoc), m_shift(orig.m_shift)
	{}
	
	void display()
	{
		auto model_matrix = glm::scale(glm::mat4(1.0f),glm::vec3(1.0f,1.0f,1.0f));
		model_matrix = glm::translate(model_matrix,m_shift);
	
		glUniformMatrix4fv(m_matrixLoc,1,GL_FALSE,(GLfloat*)&model_matrix[0]);
		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	}
private:
	GLfloat tri_vertices[12] = {
		-1.0f, -1.0f, 0.0f, 1.0f,
		1.0f, -1.0f, 0.0f, 1.0f,
		-1.0f, 1.0f, 0.0f, 1.0f
		};
	
	
	GLuint m_program;
	
	int m_positionLoc;
	int m_colorLoc;
	GLint m_matrixLoc;
	
	glm::vec4 m_shift;
};
#endif
