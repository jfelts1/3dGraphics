#pragma once
#define GLM_FORCE_CXX11
#include <GL/glew.h>
#include <vector>
#include <glm/detail/type_vec3.hpp>
#include <tuple>
#include <utility>
#include <glm/gtx/normal.hpp>
#define NumPointsPerTriangle 3
static_assert(NumPointsPerTriangle == 3, "Triangles must have 3 points.");
#define MinValueAllowed 0.0000001f


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
	std::vector<glm::vec4> m_tangentVectorsT;
	std::vector<glm::vec4> m_tangentVectorsB;
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
	void calculateTangentVectors();

	template<typename iter>
	auto calculateTBMatrix(iter faceStart)
	{
        auto i0 = faceStart[0];
        auto i1 = faceStart[1];
        auto i2 = faceStart[2];
        auto p0 = m_vertices[i0];
        auto p1 = m_vertices[i1];
        auto p2 = m_vertices[i2];

        auto u0 = m_textures[i0].x;
        auto u1 = m_textures[i1].x;
        auto u2 = m_textures[i2].x;
        auto v0 = m_textures[i0].y;
        auto v1 = m_textures[i1].y;
        auto v2 = m_textures[i2].y;

		glm::vec3 Q1(p1 - p0);
		glm::vec3 Q2(p2 - p0);
		auto s1 = u1 - u0;
		auto s2 = u2 - u0;
		auto t1 = v1 - v0;
		auto t2 = v2 - v0;
        auto inv = 1.0f / (s1*t2 - s2*t1);
        return std::make_pair(glm::vec3(t2*Q1.x-t1*Q2.x,t2*Q1.y-t1*Q2.y,t2*Q1.z-t1*Q2.z)*inv,
                              glm::vec3(s1*Q2.x-s2*Q1.x,s1*Q2.y-s2*Q1.y,s1*Q2.z-s2*Q1.z)*inv);
	}

};
