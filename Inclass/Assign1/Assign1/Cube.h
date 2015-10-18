#ifndef CUBE_H
#define CUBE_H
#ifdef _MSC_VER
#pragma warning(disable: 4505 4514 4668 4820 4710 4711)
#endif
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <array>
#define GLM_FORCE_RADIANS 
#define GLM_FORCE_CXX11
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include "Utils/ShaderUtils.h"
#define NUM_VERTEX 16
#define NUM_NORMALS 12
#define NUM_INDICES 6
#define COLOR_ARRAY_SIZE 4
#define POSITION_ARRAY_SIZE 4
class Cube
{
public:
	Cube();

	explicit Cube(std::array<GLfloat, NUM_VERTEX> vertexes, std::array<GLfloat, NUM_NORMALS> normals, std::array<GLushort, NUM_INDICES> indices,
		std::array<GLfloat, COLOR_ARRAY_SIZE> color, std::array<GLfloat, POSITION_ARRAY_SIZE> position)
		noexcept: m_vertexes(vertexes), m_normals(normals), m_indices(indices), m_color(color), m_position(position)
	{
		// ReSharper disable once CppUseAuto
		GLintptr offset = 0;
		glGenVertexArrays(1, &cube_vao);
		glBindVertexArray(cube_vao);
		glGenBuffers(1, &cube_vbo);

		glBindBuffer(GL_ARRAY_BUFFER, cube_vbo);
		glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(m_vertexes.size() + m_normals.size()), nullptr, GL_STATIC_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, offset, static_cast<GLsizeiptr>(m_vertexes.size()), m_vertexes.data());
		offset += static_cast<GLintptr>(m_vertexes.size());
		glBufferSubData(GL_ARRAY_BUFFER, offset, static_cast<GLsizeiptr>(m_normals.size()), m_normals.data());


		glGenBuffers(1, &cube_ebo);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cube_ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(m_indices.size()), m_indices.data(), GL_STATIC_DRAW);

		glVertexAttribPointer(static_cast<GLuint>(m_cubeLoc), 4, GL_FLOAT, GL_FALSE, 0, nullptr);
		glEnableVertexAttribArray(static_cast<GLuint>(m_cubeLoc));

		glVertexAttribPointer(static_cast<GLuint>(m_cubeNormalLoc), 3, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<GLvoid *>(m_vertexes.size()));
		glEnableVertexAttribArray(static_cast<GLuint>(m_cubeNormalLoc));
	};
	virtual ~Cube();

	static void initCubeShaders() noexcept
	{
		m_cubeProgram = Utils::initShaders("cube_shader.vs", "cube_shader.fs");

		// attribute indices

		m_cubeLoc = static_cast<GLuint>(glGetAttribLocation(m_cubeProgram, "position"));
		m_cubeNormalLoc = static_cast<GLuint>(glGetAttribLocation(m_cubeProgram, "normal"));

		// uniform indices

		m_cubeMatrixLoc = static_cast<GLuint>(glGetUniformLocation(m_cubeProgram, "model_matrix"));
		m_cubeColorLoc = static_cast<GLuint>(glGetUniformLocation(m_cubeProgram, "color"));
	}

	void display() const noexcept
	{
		glBindVertexArray(cube_vao);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cube_ebo);

		// Draw the center cube

		glm::mat4 model_matrix;
		glUniformMatrix4fv(static_cast<GLint>(m_cubeMatrixLoc), 1, GL_FALSE, reinterpret_cast<GLfloat*>(&model_matrix[0]));
		glUniform4fv(static_cast<GLint>(m_cubeColorLoc), 1, const_cast<GLfloat*>(&m_color[0]));
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, nullptr);
	}
private:
	std::array<GLfloat, NUM_VERTEX> m_vertexes;
	std::array<GLfloat, NUM_NORMALS> m_normals;
	std::array<GLushort, NUM_INDICES> m_indices;
	std::array<GLfloat, COLOR_ARRAY_SIZE> m_color;
	std::array<GLfloat, POSITION_ARRAY_SIZE> m_position;

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
