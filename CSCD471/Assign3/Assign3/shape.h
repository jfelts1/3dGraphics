#pragma once
#include <GL/glew.h>
#include <vector>
#include <array>

class Shape
{
public:
    Shape() = default;
    Shape(const std::vector<float>& vertices,
          const std::vector<float>& normals,
          const std::vector<float>& textures,
          const std::vector<GLuint>& indices) :
        m_vertices{vertices},
        m_normals{normals},
        m_textures{textures},
        m_indices{indices}
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

    void render();

private:
    std::vector<float> m_vertices;
    std::vector<float> m_normals;
    std::vector<float> m_textures;
    std::vector<GLuint> m_indices;
    GLuint m_vao;

};
