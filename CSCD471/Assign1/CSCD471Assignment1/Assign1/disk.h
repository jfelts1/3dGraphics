#pragma once
#include <GL/glew.h>
#include <array>

#define GLM_FORCE_RADIANS 
#define GLM_FORCE_CXX11

#include <glm/glm.hpp>

#include "objloader.h"
#define NumConePoints  18
#define NumTriangles   18
#define NumIndices NumPointsPerTriangle*NumTriangles

class Disk
{
public:
	Disk() = default;

	explicit Disk(const float yValue)
	{
		initializeCone(yValue);
	}

	auto getIndices()const
	{
		return indices;
	}

	auto getPoints()const
	{
		return points;
	}

	auto getNormals()const
	{
		return normals;
	}

	void renderInit();
	void display() const;
private:
	GLuint index = 0;
	std::array<GLuint,NumIndices> indices;
	std::array<glm::vec3, NumConePoints + 1> points;
	std::array<glm::vec3, NumConePoints + 1> normals;

	GLuint vao;
	GLuint vbo;
	GLuint ebo;

	GLuint vertexPosition;
	GLuint vertexNormal;
	
	void initializeCone(const float yValue);
	void calculateNormals();

};
