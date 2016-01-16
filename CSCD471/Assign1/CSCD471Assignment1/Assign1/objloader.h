#ifndef OBJLOADER_H
#define OBJLOADER_H

#include <vector>
#include <array>
#include <tuple>
#include <limits>
#include <iterator>
#include <algorithm>
#include <iostream>
#define GLM_FORCE_CXX11
#include <glm/glm.hpp>
#include <glm/gtx/normal.hpp>
#include "contains.h"

#define NumPointsPerTriangle 3
static_assert(NumPointsPerTriangle == 3, "Triangles must have 3 points.");

enum XYZ
{
	X,
	Y,
	Z
};

std::ostream& operator<<(std::ostream& out, XYZ xyz);

class OBJLoader {
public:
	//! Constructor
	OBJLoader();

	//! Destructor
	~OBJLoader();


	bool load(const char *filename);

	std::vector<glm::vec3> const &getVertices() const;
	std::vector<glm::vec3> const &getNormals() const;
	std::vector<int> const &getVertexIndices() const;
	std::vector<int> const &getNormalIndices() const;
	
	std::tuple<float,float,float> getMaxXYZ()const;
	std::tuple<float,float,float> getMinXYZ()const;

	void computeNormals(std::vector<glm::vec3> const &vertices,
		std::vector<int> const &indices,
		std::vector<glm::vec3> &normals);
	void scale(const double scale_factor);
private:
	std::vector<glm::vec3> mVertices;
	std::vector<glm::vec3> mNormals;
	std::vector<int> vIndices;
	std::vector<int> nIndices;
	std::vector<std::vector<glm::vec3>> m_faceAdjancentices;

	/*glm::vec3 computeVertexNormal(const std::vector<glm::vec3> normalsToAvg) const;
	std::vector<glm::vec3> getAdjacentTriangleNormals(const size_t ind);*/
	void unitize();
	
	
	float getLargestAxisValue(const std::tuple<float,float,float> maxXYZ,const std::tuple<float,float,float> minXYZ)const;
	glm::vec3 getOffsetFromCenter(const std::tuple<float,float,float> maxXYZ, const std::tuple<float,float,float> minXYZ)const;
	void centerObject(const glm::vec3 offset);
};

	glm::vec3 computeVertexNormal(const std::vector<glm::vec3> normalsToAvg);
	std::vector<glm::vec3> getAdjacentTriangleNormals(const size_t ind,
	const std::vector<glm::vec3>& vertices,
	const std::vector<int>& indices);

#endif

