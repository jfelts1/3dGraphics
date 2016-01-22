#pragma once
#include "objloader.h"

inline void deformModel(std::vector<glm::vec3>& vertices, std::vector<glm::vec3>& normals,const float amount)
{
	for (size_t i = 0;i < vertices.size();i++)
	{
		vertices[i] += amount*normals[i];
	}
}
