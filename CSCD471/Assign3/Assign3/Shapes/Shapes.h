#pragma once
#include <array>
#include "shape.h"

#define PI 3.1415926535897932384626433832795
#define TWOPI 2*PI
#define NUM_HORIZONTAL_SLICES_SPHERE 10
#define NUM_VERTICAL_SLICES_SPHERE 10
#define SPHERE_TOP_Y 1.0f

namespace Shapes 
{
	Shape makeCube(const glm::vec3 position= glm::vec3(0), const float scale=1.0f);
	Shape makePyramid(const glm::vec3 position = glm::vec3(0), const float scale = 1.0f);
	Shape makeSphere(const glm::vec3 position = glm::vec3(0), const float scale = 1.0f);
    namespace Sphere
    {
        std::array<std::vector<glm::vec3>,NUM_HORIZONTAL_SLICES_SPHERE> calculatePoints();
        void constructSphere(const std::array<std::vector<glm::vec3>,NUM_HORIZONTAL_SLICES_SPHERE>& sphereMiddle,
                             std::vector<glm::vec3>& vertices,
                             std::vector<GLuint>& indices);
    }
}
