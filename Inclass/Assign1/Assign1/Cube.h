#ifndef CUBE_H
#define CUBE_H
#ifdef _MSC_VER
#pragma warning(disable: 4505 4514 4668 4820 4710 4711)
#endif
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <array>
#include <vector>
#include <cassert>
#include <cstdio>
#include <iostream>
#include <tuple>
#define GLM_FORCE_RADIANS 
#define GLM_FORCE_CXX11
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include "Utils/ShaderUtils.h"
#define NUM_FACES 6
#define NUM_VERTEX 16*NUM_FACES
#define NUM_NORMALS 12*NUM_FACES
#define NUM_INDICES 6*NUM_FACES
#define COLOR_ARRAY_SIZE 4
#define POSITION_ARRAY_SIZE 4

void printMat4(const glm::mat4 m);


class Cube
{
public:
	Cube();

	explicit Cube(
		const std::array<GLfloat, COLOR_ARRAY_SIZE>& color, 
		const std::array<GLfloat, POSITION_ARRAY_SIZE>& position)
		noexcept: m_color(color), m_position(position)
	{
		//asserts to make sure the vectors are of the correct sizes
		assert(m_vertexes.size() == NUM_VERTEX);
		assert(m_normals.size() == NUM_NORMALS);
		assert(m_indices.size() == NUM_INDICES);

		m_scale = glm::vec3(1, 1, 1);
		m_axisOfRot = glm::vec3(1,1,1);
		m_angle = 0;
		m_xAxisMult = 1;
		m_yAxisMult = 1;
		m_zAxisMult = 1;
		m_xAngle = 0;
		m_yAngle = 0;
		m_zAngle = 0;
	};
	
	virtual ~Cube();

	void setScale(const glm::vec3 scale)noexcept { m_scale = scale; }
	void setRotate(const glm::vec3 rotAxis, const float angle) noexcept
	{
		m_axisOfRot = rotAxis;
		m_angle = angle;
	}
	
	void setRotateX(const float magnitude, const float angle) noexcept
	{
		m_xAxisMult = magnitude;
		m_xAngle = angle;
	}
	
	void setRotateY(const float magnitude, const float angle) noexcept
	{
		m_yAxisMult = magnitude;
		m_yAngle = angle;
	}
	
	void setRotateZ(const float magnitude, const float angle) noexcept
	{
		m_zAxisMult = magnitude;
		m_zAngle = angle;
	}

	static void initCubeShaders() noexcept
	{
		m_cubeProgram = Utils::initShaders("cube_shader.vs", "cube_shader.fs");

		// attribute indices

		m_cubeLoc = static_cast<GLuint>(glGetAttribLocation(m_cubeProgram, "position"));
		m_cubeNormalLoc = static_cast<GLuint>(glGetAttribLocation(m_cubeProgram, "normal"));

		// uniform indices

		m_cubeMatrixLoc = static_cast<GLuint>(glGetUniformLocation(m_cubeProgram, "model_matrix"));
		m_cubeColorLoc = static_cast<GLuint>(glGetUniformLocation(m_cubeProgram, "color"));

		GLintptr offset = 0;
		glGenVertexArrays(1, &cube_vao);
		glBindVertexArray(cube_vao);
		glGenBuffers(1, &cube_vbo);

		glBindBuffer(GL_ARRAY_BUFFER, cube_vbo);
		glBufferData(
			GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(m_vertexes.size()*sizeof(GLfloat) + m_normals.size()*sizeof(GLfloat)), nullptr, GL_STATIC_DRAW);

		glBufferSubData(GL_ARRAY_BUFFER, offset, static_cast<GLsizeiptr>(m_vertexes.size()*sizeof(GLfloat)), m_vertexes.data());
		offset += static_cast<GLintptr>(m_vertexes.size()*sizeof(GLfloat));
		glBufferSubData(GL_ARRAY_BUFFER, offset, static_cast<GLsizeiptr>(m_normals.size()*sizeof(GLfloat)), m_normals.data());

		glGenBuffers(1, &cube_ebo);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cube_ebo);
		glBufferData(
			GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(m_indices.size()*sizeof(GLushort)), m_indices.data(), GL_STATIC_DRAW);

		glVertexAttribPointer(static_cast<GLuint>(m_cubeLoc), 4, GL_FLOAT, GL_FALSE, 0, nullptr);
		glEnableVertexAttribArray(static_cast<GLuint>(m_cubeLoc));

		glVertexAttribPointer(
			static_cast<GLuint>(m_cubeNormalLoc), 3, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<GLvoid *>(m_vertexes.size()*sizeof(GLfloat)));

		glEnableVertexAttribArray(static_cast<GLuint>(m_cubeNormalLoc));
	}

	static void bind()noexcept
	{
		glBindVertexArray(cube_vao);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cube_ebo);
	}

	void display() const noexcept
	{
		glm::mat4 model_matrix = translate(glm::mat4(1.0f), glm::vec3(m_position[0], m_position[1], m_position[2]));
		//ugly hack
		if(m_yAngle == 0 && m_zAngle == 0)
		{
			//blue
			model_matrix = rotate(model_matrix,m_xAngle,m_xAxis*m_xAxisMult);
		}
		else
		{
			model_matrix *= rotate(glm::mat4(1.0f),-acosf(3.0f/sqrtf(13)), m_xAxis*m_xAxisMult)
				*rotate(glm::mat4(1.0f), acosf(sqrtf(13.0f/14.0f)), m_yAxis*m_yAxisMult)
				*rotate(glm::mat4(1.0f), m_zAngle, m_zAxis*m_zAxisMult)
				*rotate(glm::mat4(1.0f), -acosf(sqrtf(13.0f / 14.0f)), m_yAxis*m_yAxisMult)
				*rotate(glm::mat4(1.0f), acosf(3.0f / sqrtf(13)), m_xAxis*m_xAxisMult);

		}
		model_matrix = scale(model_matrix, m_scale);
		
		glUniformMatrix4fv(
				static_cast<GLint>(m_cubeMatrixLoc), 1, GL_FALSE, reinterpret_cast<GLfloat*>(&model_matrix[0]));

		glUniform4fv(static_cast<GLint>(m_cubeColorLoc), 1, const_cast<GLfloat*>(m_color.data()));
		glDrawElements(GL_TRIANGLES, NUM_INDICES, GL_UNSIGNED_SHORT, nullptr);
	}

private:
	static const std::vector<GLfloat> m_vertexes;
	static const std::vector<GLfloat> m_normals;
	static const std::vector<GLushort> m_indices;
	std::array<GLfloat, COLOR_ARRAY_SIZE> m_color;
	std::array<GLfloat, POSITION_ARRAY_SIZE> m_position;
	glm::vec3 m_scale;
	glm::vec3 m_axisOfRot;
	
	static const glm::vec3 m_xAxis;
	float m_xAxisMult;
	float m_xAngle;
	static const glm::vec3 m_yAxis;
	float m_yAxisMult;
	float m_yAngle;
	static const glm::vec3 m_zAxis;
	float m_zAxisMult;
	float m_zAngle;
	float m_angle;

	static GLuint m_cubeProgram;
	static GLuint m_cubeLoc;
	static GLuint m_cubeNormalLoc;
	static GLuint m_cubeMatrixLoc;
	static GLuint m_cubeColorLoc;

	static GLuint cube_vao;
	static GLuint cube_vbo;
	static GLuint cube_ebo;
	
};
#endif
