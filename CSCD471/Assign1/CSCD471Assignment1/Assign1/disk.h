#pragma once
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>
#include <cmath>
#include <array>
#include <vector>
#include <algorithm>
#include <iterator>

#define GLM_FORCE_RADIANS 
#define GLM_FORCE_CXX11

#include <glm/glm.hpp>
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtx/normal.hpp>

#include "objloader.h"
#define NumConePoints  18
#define NumTriangles   18
#define NumIndices NumPointsPerTriangle*NumTriangles

class Disk
{
public:
	Disk(const float yValue)
	{
		
	}
private:
	static constexpr double kPI = 3.1415926535897932384626433832795;
	GLuint index = 0;
	std::array<GLuint,NumIndices> indices;
	std::array<glm::vec3, NumConePoints + 1> points;
	std::array<glm::vec3, NumConePoints + 1> normals;
	
	void initializeCone(const float yValue)
	{
		//top point of cone
		points[index][0] = 0.0;
		points[index][1] = yValue;
		points[index][2] = 0.0;
		//points[index][3] = 1.0;

		normals[index][0] = 0.0;
		normals[index][1] = 0.0;
		normals[index][2] = 0.0;

		index++;

		float theta;
		size_t tIndices = 0;

		for (size_t i = 0; i < NumConePoints; ++i, index++)
		{
			theta = static_cast<float>(i*20.0f*kPI / 180.0f);

			points[index][0] = cos(theta);
			points[index][1] = yValue;
			points[index][2] = -sin(theta);
			//points[index][3] = 1.0;

			normals[index][0] = 0.0;
			normals[index][1] = yValue;
			normals[index][2] = 0.0;

			if (i <= (NumConePoints - 2))
			{
				indices[tIndices] = 0u;
				tIndices++;
				indices[tIndices] = index;
				tIndices++;
				indices[tIndices] = index + 1;
				tIndices++;
			}
			//last triangle
			else
			{
				indices[tIndices] = 0u;
				tIndices++;
				indices[tIndices] = index;
				tIndices++;
				indices[tIndices] = 1u;
				tIndices++;
			}
		}
		std::vector<glm::vec3> vertices;
		for(auto& p: points)
		{
			vertices.emplace_back(p);
		}
		std::vector<int> ind;
		for(auto& i:indices)
		{
			ind.emplace_back(i);
		}
		
		for (size_t i = 0;i < points.size();i++)
		{
			std::vector<glm::vec3> normalsVec = getAdjacentTriangleNormals(i,vertices,ind);
			normals[i] = computeVertexNormal(normalsVec);
			//printf("%llu: %f,%f,%f\n",i,normals[i][0],normals[i][1],normals[i][2]);
			normalsVec.clear();
		}
	}
};
