#pragma once
#include <array>
#include "../shape.h"

#define PI 3.1415926535897932384626433832795
#define TWOPI 2*PI

namespace Shapes 
{
	Shape makeCube(const glm::vec3 position= glm::vec3(0), const float scale=1.0f);
}
