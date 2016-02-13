#ifndef OBJLOADER_H
#define OBJLOADER_H
#include <string>
#include <vector>
#define GLM_FORCE_CXX11
#include <GL/glew.h>
#include <glm/detail/type_vec3.hpp>

class objLoader
{
public:
	explicit objLoader(const std::string& filepath);
    void print_data()const
    {
        puts(m_data.c_str());
    }
    std::vector<glm::vec3> getVertices() const;
    std::vector<glm::vec3> getNormals() const;
    std::vector<float> getTextures()const;
    std::vector<GLuint> getIndices()const;

private:
    std::string m_data;
};

#endif // OBJLOADER_H
