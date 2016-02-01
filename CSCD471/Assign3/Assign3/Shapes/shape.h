#pragma once
#define GLM_FORCE_CXX11
#include <GL/glew.h>
#include <vector>
#include <array>
#include <glm/detail/type_vec3.hpp>
#include <tuple>
#include <limits>
#include <algorithm>

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
          const std::vector<float>& textures,
          const std::vector<GLuint>& indices,
		  const glm::vec3 position = glm::vec3(0),
		  const float scale_factor = 1.0f) :
        m_vertices{vertices},
        m_normals{normals},
        m_textures{textures},
        m_indices{indices}
    {
		unitize();
		scaleShape(scale_factor);
		shiftShape(position);
	    initShapeRender();
    }

	virtual ~Shape() = default;
    void render() const;
	void shiftShape(const glm::vec3 offset);
	void scaleShape(const float scale_factor);
	std::tuple<float, float, float> getMaxXYZ()const;
	std::tuple<float, float, float> getMinXYZ()const;
private:
    std::vector<glm::vec3> m_vertices;
    std::vector<glm::vec3> m_normals;
    std::vector<float> m_textures;
    std::vector<GLuint> m_indices;
    GLuint m_vao;

	void unitize();
	float getLargestAxisValue(const std::tuple<float, float, float> maxXYZ, const std::tuple<float, float, float> minXYZ)const;
	glm::vec3 getOffsetFromCenter(const std::tuple<float, float, float> maxXYZ, const std::tuple<float, float, float> minXYZ)const;
	void initShapeRender();
};
