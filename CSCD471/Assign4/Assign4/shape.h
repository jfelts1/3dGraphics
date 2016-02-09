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
          const std::vector<float>& textures,
          const std::vector<GLuint>& indices,
		  const glm::vec3 position = glm::vec3(0),
		  const float scale_factor = 1.0f) :
        m_vertices{vertices},
        m_normals{normals},
        m_textures{textures},
        m_indices{indices}
    {
        commonInitShape(position,scale_factor);
    }

    Shape(const std::vector<glm::vec3>& vertices,
          const std::vector<float>& textures,
          const std::vector<GLuint>& indices,
          const glm::vec3 position = glm::vec3(0),
          const float scale_factor = 1.0f):
        m_vertices{vertices},
        m_textures{textures},
        m_indices{indices}
    {
        calculateNormals();
        commonInitShape(position,scale_factor);
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
    void commonInitShape(const glm::vec3 position, const float scale_factor)
    {
        unitize();
        scaleShape(scale_factor);
        shiftShape(position);
        initShapeRender();
    }

    void calculateNormals()
    {
        m_normals.resize(m_vertices.size(), glm::vec3(0.0f, 0.0f, 0.0f));

            // Compute per-vertex normals here!
            for (size_t i = 0;i < m_vertices.size();i++)
            {
                std::vector<glm::vec3> normalsVec = getAdjacentTriangleNormals(i,m_vertices,m_indices);
                m_normals[i] = computeVertexNormal(normalsVec);
                //printf("%llu: %f,%f,%f\n",i,normals[i][0],normals[i][1],normals[i][2]);
                normalsVec.clear();
            }
    }
    glm::vec3 computeVertexNormal(const std::vector<glm::vec3> normalsToAvg)
    {
        glm::vec3 ret;
        for (auto& norm : normalsToAvg)
        {
            ret += norm;
        }
        return normalize(ret);
    }

    std::vector<glm::vec3> getAdjacentTriangleNormals(const size_t ind,
        const std::vector<glm::vec3>& vertices,
        const std::vector<GLuint>& indices)
    {
        std::vector<glm::vec3> ret;
        for (auto beg = indices.begin(), end = indices.end();beg != end;advance(beg, NumPointsPerTriangle))
        {
            //if (contains(beg, beg + NumPointsPerTriangle, ind))
            //considerably faster to do it this way
            if((beg[0]==ind)||(beg[1]==ind)||(beg[2]==ind))
            {
                ret.emplace_back(triangleNormal(glm::vec3(vertices[beg[0]]), glm::vec3(vertices[beg[1]]), glm::vec3(vertices[beg[2]])));
            }
        }
        return ret;
    }
};
