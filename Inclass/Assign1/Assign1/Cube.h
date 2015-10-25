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

		GLintptr offset = 0;
		glGenVertexArrays(1, &cube_vao);
		glBindVertexArray(cube_vao);
		glGenBuffers(1, &cube_vbo);

		glBindBuffer(GL_ARRAY_BUFFER, cube_vbo);
		glBufferData(
			GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(NUM_VERTEX + NUM_NORMALS), nullptr, GL_STATIC_DRAW);

		glBufferSubData(GL_ARRAY_BUFFER, offset, static_cast<GLsizeiptr>(NUM_VERTEX), m_vertexes.data());
		offset += static_cast<GLintptr>(m_vertexes.size());
		glBufferSubData(GL_ARRAY_BUFFER, offset, static_cast<GLsizeiptr>(NUM_NORMALS), m_normals.data());

		glGenBuffers(1, &cube_ebo);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cube_ebo);
		glBufferData(
			GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(m_indices.size()), m_indices.data(), GL_STATIC_DRAW);

		glVertexAttribPointer(static_cast<GLuint>(m_cubeLoc), 4, GL_FLOAT, GL_FALSE, 0, nullptr);
		glEnableVertexAttribArray(static_cast<GLuint>(m_cubeLoc));

		glVertexAttribPointer(
			static_cast<GLuint>(
			m_cubeNormalLoc), 3, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<GLvoid *>(m_vertexes.size()));
		
		glEnableVertexAttribArray(static_cast<GLuint>(m_cubeNormalLoc));

		m_scale = glm::vec3(1, 1, 1);
	};
	virtual ~Cube();

	void setScale(const glm::vec3 scale)noexcept { m_scale = scale; }

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

		for (size_t i = 0;i < NUM_FACES;i++)
		{
			glm::mat4 model_matrix = scale(glm::mat4(test[i*4], test[(i*4)+1], test[(i*4)+2], test[(i*4)+3]), m_scale);
			model_matrix += translate(glm::mat4(1.0f), glm::vec3(m_position[0], m_position[1], m_position[2]));
			glUniformMatrix4fv(
				static_cast<GLint>(m_cubeMatrixLoc), 1, GL_FALSE, reinterpret_cast<GLfloat*>(&model_matrix[0]));
		}

		glUniform4fv(static_cast<GLint>(m_cubeColorLoc), 1, const_cast<GLfloat*>(&m_color[0]));
		glDrawElements(GL_TRIANGLES, NUM_VERTEX, GL_UNSIGNED_SHORT, nullptr);
	}
private:
	static const std::vector<GLfloat> m_vertexes;
	static const std::vector<GLfloat> m_normals;
	static const std::vector<GLushort> m_indices;
	static const std::vector<glm::vec4> test;
	std::array<GLfloat, COLOR_ARRAY_SIZE> m_color;
	std::array<GLfloat, POSITION_ARRAY_SIZE> m_position;
	glm::vec3 m_scale;

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
