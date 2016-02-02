#include <iostream>
#include <limits>
#include <algorithm>
#include <array>
#include "shape.h"

using std::tuple;
using std::numeric_limits;
using std::get;
using std::make_tuple;
using std::distance;
using std::max_element;
using std::array;

void Shape::render() const
{
    glBindVertexArray(m_vao);
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(m_indices.size()), GL_UNSIGNED_INT, nullptr);
}

void Shape::shiftShape(const glm::vec3 offset)
{
	for (auto& vec : m_vertices)
	{
		vec -= offset;
	}
}

void Shape::scaleShape(const float scale_factor)
{
	for (auto& vec : m_vertices)
	{
		vec *= scale_factor;
	}
}

void Shape::initShapeRender()
{
	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	unsigned int handle[4];
	glGenBuffers(4, handle);

	glBindBuffer(GL_ARRAY_BUFFER, handle[0]);
	glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(decltype(m_vertices[0])), m_vertices.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(static_cast<GLuint>(0), 3, GL_FLOAT, GL_FALSE, 0, nullptr);
	glEnableVertexAttribArray(0);  // Vertex position

	glBindBuffer(GL_ARRAY_BUFFER, handle[1]);
	glBufferData(GL_ARRAY_BUFFER, m_normals.size() * sizeof(decltype(m_normals[0])), m_normals.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(static_cast<GLuint>(1), 3, GL_FLOAT, GL_FALSE, 0, nullptr);
	glEnableVertexAttribArray(1);  // Vertex normal

	glBindBuffer(GL_ARRAY_BUFFER, handle[2]);
	glBufferData(GL_ARRAY_BUFFER, m_textures.size() * sizeof(decltype(m_textures[0])), m_textures.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(static_cast<GLuint>(2), 2, GL_FLOAT, GL_FALSE, 0, nullptr);
	glEnableVertexAttribArray(2);  // texture coords

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handle[3]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(decltype(m_indices[0])), m_indices.data(), GL_STATIC_DRAW);

	glBindVertexArray(0);
}

tuple<float, float, float> Shape::getMaxXYZ() const
{
	float x = -numeric_limits<float>::infinity();
	float y = -numeric_limits<float>::infinity();
	float z = -numeric_limits<float>::infinity();

	for (auto& vec : m_vertices)
	{
		if (vec[0] > x)
		{
			x = vec[0];
		}
		if (vec[1]>y)
		{
			y = vec[1];
		}
		if (vec[2]>z)
		{
			z = vec[2];
		}
	}

	return make_tuple(x, y, z);
}

tuple<float, float, float> Shape::getMinXYZ() const
{
	float x = numeric_limits<float>::infinity();
	float y = numeric_limits<float>::infinity();
	float z = numeric_limits<float>::infinity();

	for (auto&vec : m_vertices)
	{
		if (vec[0]<x)
		{
			x = vec[0];
		}
		if (vec[1]<y)
		{
			y = vec[1];
		}
		if (vec[2]<z)
		{
			z = vec[2];
		}
	}

	return make_tuple(x, y, z);
}

void Shape::unitize()
{
	auto maxPos = getMaxXYZ();
	auto minPos = getMinXYZ();

	printf("max:\nx: %f, y: %f, z: %f\n", get<0>(maxPos), get<1>(maxPos), get<2>(maxPos));
	printf("min:\nx: %f, y: %f, z: %f\n", get<0>(minPos), get<1>(minPos), get<2>(minPos));

	float largestAxis = getLargestAxisValue(maxPos, minPos);
	glm::vec3 offsetFromCenter = getOffsetFromCenter(maxPos, minPos);
	shiftShape(offsetFromCenter);
	float scale_factor = 2.0f / largestAxis;

	printf("scale_factor: %f\n", scale_factor);

	scaleShape(scale_factor);
}

float Shape::getLargestAxisValue(const tuple<float, float, float> maxXYZ, const tuple<float, float, float> minXYZ) const
{
	XYZ axis;
	float x = get<0>(maxXYZ) - get<0>(minXYZ);
	float y = get<1>(maxXYZ) - get<1>(minXYZ);
	float z = get<2>(maxXYZ) - get<2>(minXYZ);
	array<float, 3> tmp{ x,y,z };
	auto result = max_element(tmp.begin(), tmp.end());
	auto largestAxisIndex = distance(tmp.begin(), result);
	if (largestAxisIndex == 0)
	{
		axis = X;
	}
	else if (largestAxisIndex == 1)
	{
		axis = Y;
	}
	else
	{
		axis = Z;
	}
	std::cout << "Largest axis is " << axis << " with length of " << *result << "." << std::endl;
	return *result;
}

glm::vec3 Shape::getOffsetFromCenter(const tuple<float, float, float> maxXYZ, const tuple<float, float, float> minXYZ) const
{
	float x = (get<0>(maxXYZ) + get<0>(minXYZ)) / 2.0f;
	float y = (get<1>(maxXYZ) + get<1>(minXYZ)) / 2.0f;
	float z = (get<2>(maxXYZ) + get<2>(minXYZ)) / 2.0f;
	return glm::vec3(x, y, z);
}

std::ostream& operator<<(std::ostream& out, XYZ xyz)
{
	switch (xyz)
	{
	case X:
		out << "X";
		break;
	case Y:
		out << "Y";
		break;
	case Z:
		out << "Z";
		break;
	}
	return out;
}