#pragma once
#define GLM_FORCE_CXX11
#include <GL/glew.h>
#include <vector>
#include <glm/detail/type_vec3.hpp>
#include <tuple>
#include <glm/gtx/normal.hpp>
#define NumPointsPerTriangle 3
static_assert(NumPointsPerTriangle == 3, "Triangles must have 3 points.");


enum XYZ
{
	X,
	Y,
	Z
};

std::ostream& operator<<(std::ostream& out, XYZ xyz);

class Shape
{
public:
    Shape() = default;

	Shape(const std::vector<glm::vec3>& vertices,
	      const std::vector<glm::vec3>& normals,
	      const std::vector<glm::vec2>& textures,
	      const std::vector<GLuint>& indices,
	      const glm::vec3 position = glm::vec3(0),
	      const float scale_factor = 1.0f);

	Shape(const std::vector<glm::vec3>& vertices,
	      const std::vector<glm::vec2>& textures,
	      const std::vector<GLuint>& indices,
	      const glm::vec3 position = glm::vec3(0),
	      const float scale_factor = 1.0f);

	virtual ~Shape() = default;
    void render() const;
	void shiftShape(const glm::vec3 offset);
	void scaleShape(const float scale_factor);
	std::tuple<float, float, float> getMaxXYZ()const;
	std::tuple<float, float, float> getMinXYZ()const;
private:
    std::vector<glm::vec3> m_vertices;
    std::vector<glm::vec3> m_normals;
    std::vector<glm::vec2> m_textures;
	std::vector<glm::mat3> m_tangentSpaceTransformationMats;
    std::vector<GLuint> m_indices;
    GLuint m_vao;

	void unitize();

	float getLargestAxisValue(const std::tuple<float, float, float> maxXYZ,
		const std::tuple<float, float, float> minXYZ)const;
	glm::vec3 getOffsetFromCenter(const std::tuple<float, float, float> maxXYZ,
		const std::tuple<float, float, float> minXYZ)const;

	void initShapeRender();

	void commonInitShape(const glm::vec3 position, const float scale_factor);

	void calculateNormals();	
	void calculateTangentSpace();
	std::vector<std::pair<glm::vec3, glm::vec3>> calculateTangentVectors();

	template<typename iter>
	auto calculateTBMatrix(iter faceStart)
	{
		auto p0 = m_vertices[faceStart[0]];
		auto p1 = m_vertices[faceStart[1]];
		auto p2 = m_vertices[faceStart[2]];

		auto u0 = m_textures[faceStart[0]].x;
		auto u1 = m_textures[faceStart[1]].x;
		auto u2 = m_textures[faceStart[2]].x;
		auto v0 = m_textures[faceStart[0]].y;
		auto v1 = m_textures[faceStart[1]].y;
		auto v2 = m_textures[faceStart[2]].y;

		glm::vec3 Q1(p1 - p0);
		glm::vec3 Q2(p2 - p0);
		auto s1 = u1 - u0;
		auto s2 = u2 - u0;
		auto t1 = v1 - v0;
		auto t2 = v2 - v0;
		return 1.0f / (s1*t2 - s2*t1)*(glm::mat2x3(Q1.x, Q2.x, Q1.y, Q2.y, Q1.z, Q2.z)*glm::mat2(t2, -s2, -t1, s1));
	}

};
