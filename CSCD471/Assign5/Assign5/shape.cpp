#include <iostream>
#include <limits>
#include <algorithm>
#include <array>
#include <vector>
#include "shape.h"
#include "Debug.h"

using std::tuple;
using std::numeric_limits;
using std::get;
using std::make_tuple;
using std::distance;
using std::max_element;
using std::array;
using std::vector;
using glm::vec3;
using glm::vec2;
using glm::vec4;
using std::pair;
using glm::normalize;
using glm::cross;
using glm::dot;
using glm::triangleNormal;

Shape::Shape(const vector<vec3>& vertices,
	const vector<vec3>& normals,
	const vector<vec2>& textures,
	const vector<GLuint>& indices,
	const vec3 position,
	const float scale_factor) :
	m_vertices{ vertices },
	m_normals{ normals },
	m_textures{ textures },
	m_indices{ indices }
{
	commonInitShape(position, scale_factor);
}

Shape::Shape(const vector<vec3>& vertices,
	const vector<vec2>& textures,
	const vector<GLuint>& indices,
	const vec3 position,
	const float scale_factor) :
	m_vertices{ vertices },
	m_textures{ textures },
	m_indices{ indices }
{
	calculateNormals();
	commonInitShape(position, scale_factor);
}

void Shape::render() const
{
    /*for(size_t i =0;i<m_indices.size();i+=3)
    {
        /*printf("I1:%i I2:%i I3:%i v1[%.2f,%.2f,%.2f] v2[%.2f,%.2f,%.2f] v3[%.2f,%.2f,%.2f]\n",m_indices[i],m_indices[i+1],m_indices[i+2],
		m_vertices[m_indices[i]][0], m_vertices[m_indices[i]][1], m_vertices[m_indices[i]][2],
		m_vertices[m_indices[i+1]][0], m_vertices[m_indices[i+1]][1], m_vertices[m_indices[i+1]][2],
		m_vertices[m_indices[i + 2]][0], m_vertices[m_indices[i + 2]][1], m_vertices[m_indices[i + 2]][2]);*/
		/*printf("I1:%3i I2:%3i I3:%3i\n", m_indices[i], m_indices[i + 1], m_indices[i + 2]);
    }*/
    glBindVertexArray(m_vao);
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(m_indices.size()), GL_UNSIGNED_INT, nullptr);
}

void Shape::shiftShape(const vec3 offset)
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

	unsigned int handle[5];
	glGenBuffers(5, handle);

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

	glBindBuffer(GL_ARRAY_BUFFER, handle[3]);
	glBufferData(GL_ARRAY_BUFFER, m_tangentVectorsT.size() * sizeof(decltype(m_tangentVectorsT[0])), m_tangentVectorsT.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(static_cast<GLuint>(3), 4, GL_FLOAT, GL_FALSE, 0, nullptr);
	glEnableVertexAttribArray(3);  // vertex tangent

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handle[4]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(decltype(m_indices[0])), m_indices.data(), GL_STATIC_DRAW);

	glBindVertexArray(0);
}

void Shape::commonInitShape(const vec3 position, const float scale_factor)
{
	unitize();
	scaleShape(scale_factor);
	shiftShape(position);
	calculateTangentSpace();
	initShapeRender();
}

void Shape::calculateNormals()
{
	m_normals.resize(m_vertices.size(), vec3(0.0f));
	auto maxElementIndex = distance(m_indices.begin(), max_element(m_indices.begin(), m_indices.end()));
	dprintf("m_normals.size():%llu,m_indices[maxElementIndex]+1:%llu,maxElementIndex:%llu\n", m_normals.size(), m_indices[maxElementIndex] + 1, maxElementIndex);
	assert(m_normals.size() == m_indices[maxElementIndex]+1);
	// Compute per-vertex normals here!
	for (auto beg = m_indices.begin(), end = m_indices.end();beg != end;advance(beg,NumPointsPerTriangle))
	{
		vec3 faceNorm(triangleNormal(m_vertices[beg[0]], m_vertices[beg[1]], m_vertices[beg[2]]));
		m_normals[beg[0]] += faceNorm;
		m_normals[beg[1]] += faceNorm;
		m_normals[beg[2]] += faceNorm;
	}
	for(auto& norm: m_normals)
	{
		norm = normalize(norm);
	}
}

void Shape::calculateTangentSpace()
{
	/*auto tangentVectors = calculateTangentVectors();
	dprintf("m_normals.size:%llu,tangentVectors.size():%llu\n", m_normals.size(), tangentVectors.size());
	assert(m_normals.size() == tangentVectors.size());
	m_tangentSpaceTransformationMats.resize(tangentVectors.size(), glm::mat3(0.0f));
	for (size_t i = 0;i < tangentVectors.size();i++)
	{		
		m_tangentSpaceTransformationMats[i] = inverse(glm::mat3(tangentVectors[i].first, tangentVectors[i].second, m_normals[i]));
	}*/
	//vector<pair<vec3, vec3>> tangentVectors;
	dprintf("m_texture.size():%llu,m_vertices.size():%llu,m_normals.size():%llu\n", m_textures.size(), m_vertices.size(), m_normals.size());
	assert(m_textures.size() == m_vertices.size());
	assert(m_vertices.size() == m_normals.size());
	m_tangentVectorsT.resize(m_normals.size(), vec4(0.0f));
	m_tangentVectorsB.resize(m_normals.size(), vec4(0.0f));
	for (auto beg = m_indices.begin(), end = m_indices.end();beg != end;advance(beg, NumPointsPerTriangle))
	{
		auto TB = calculateTBMatrix(beg);
        vec4 T(TB[0].x, TB[0].y, TB[0].z,0.0f);
        vec4 B(TB[1].x, TB[1].y, TB[1].z,0.0f);
        //dprintf("T{%f,%f,%f,%f}\n",T[0],T[1],T[2],T[3]);
		m_tangentVectorsT[beg[0]] += T;
		m_tangentVectorsT[beg[1]] += T;
		m_tangentVectorsT[beg[2]] += T;
		m_tangentVectorsB[beg[0]] += B;
		m_tangentVectorsB[beg[1]] += B;
		m_tangentVectorsB[beg[2]] += B;
	}

    for (size_t i = 0;i < m_tangentVectorsT.size();i++)
    {
        auto T = m_tangentVectorsT[i];
        auto N = m_normals[i];
        auto B = m_tangentVectorsB[i];
        auto NTDot = dot(N, vec3(T));
        auto NTCross = cross(N,vec3(T));
        auto NTCrossBDot = dot(NTCross,vec3(B));
        m_tangentVectorsT[i] = vec4(vec3(T)-NTDot*vec3(N),0.0f);
        m_tangentVectorsT[i].w = (NTCrossBDot<0.0f)?-1.0f:1.0f;
    }

    /*for(size_t i =0;i<m_tangentVectorsB.size();i++)
    {
        vec4 dot1(vec3(m_tangentVectorsB[i])-dot(m_normals[i],vec3(m_tangentVectorsB[i]))*m_normals[i],0.0f);
        vec4 dot2(dot(vec3(m_tangentVectorsT[i]),vec3(m_tangentVectorsB[i]))*vec3(m_tangentVectorsT[i]/(m_tangentVectorsT[i]*m_tangentVectorsT[i])),0.0f);
        m_tangentVectorsB[i] = dot1-dot2;
    }*/

    /*for (auto& vec : m_tangentVectorsT)
	{
		vec = normalize(vec);		
	}
	for (auto& vec : m_tangentVectorsB)
	{
		vec = normalize(vec);
    }*/
    for(auto& vec:m_tangentVectorsT)
    {
        dprintf("m_tangentVectorT:{%f,%f,%f,%f}\n",vec[0],vec[1],vec[2],vec[3]);

    }
}

/*vector<pair<vec3, vec3>> Shape::calculateTangentVectors()
{
	vector<pair<vec3, vec3>> tangentVectors;
	dprintf("m_texture.size():%llu,m_vertices.size():%llu,m_normals.size():%llu\n", m_textures.size(), m_vertices.size(), m_normals.size());
	assert(m_textures.size() == m_vertices.size());
	assert(m_vertices.size() == m_normals.size());
	tangentVectors.resize(m_normals.size(), pair<vec3, vec3>(vec3(0.0f), vec3(0.0f)));
	for (auto beg = m_indices.begin(), end = m_indices.end();beg != end;advance(beg, NumPointsPerTriangle))
	{
		auto TB = calculateTBMatrix(beg);
		vec3 T(TB[0].x, TB[0].y, TB[0].z);
		vec3 B(TB[1].x, TB[1].y, TB[1].z);
		tangentVectors[beg[0]].first += T;
		tangentVectors[beg[1]].first += T;
		tangentVectors[beg[2]].first += T;
		tangentVectors[beg[0]].second += B;
		tangentVectors[beg[1]].second += B;
		tangentVectors[beg[2]].second += B;
	}

	for (auto& vecs : tangentVectors)
	{
		vecs.first = normalize(vecs.first);
		vecs.second = normalize(vecs.second);
	}
	return tangentVectors;
}*/

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

	dprintf("max:\nx: %f, y: %f, z: %f\n", get<0>(maxPos), get<1>(maxPos), get<2>(maxPos));
	dprintf("min:\nx: %f, y: %f, z: %f\n", get<0>(minPos), get<1>(minPos), get<2>(minPos));

	float largestAxis = getLargestAxisValue(maxPos, minPos);
	vec3 offsetFromCenter = getOffsetFromCenter(maxPos, minPos);
	shiftShape(offsetFromCenter);
	float scale_factor = 2.0f / largestAxis;

	dprintf("scale_factor: %f\n", scale_factor);

	scaleShape(scale_factor);
}

float Shape::getLargestAxisValue(const tuple<float, float, float> maxXYZ, const tuple<float, float, float> minXYZ) const
{
	float x = get<0>(maxXYZ) - get<0>(minXYZ);
	float y = get<1>(maxXYZ) - get<1>(minXYZ);
	float z = get<2>(maxXYZ) - get<2>(minXYZ);
	array<float, 3> tmp{ x,y,z };
	auto result = max_element(tmp.begin(), tmp.end());
#ifndef NDEBUG
	XYZ axis;
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
#endif
	return *result;
}

vec3 Shape::getOffsetFromCenter(const tuple<float, float, float> maxXYZ, const tuple<float, float, float> minXYZ) const
{
	float x = (get<0>(maxXYZ) + get<0>(minXYZ)) / 2.0f;
	float y = (get<1>(maxXYZ) + get<1>(minXYZ)) / 2.0f;
	float z = (get<2>(maxXYZ) + get<2>(minXYZ)) / 2.0f;
	return vec3(x, y, z);
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
