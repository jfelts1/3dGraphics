#pragma once
#include "shape.h"

namespace Shapes 
{
	Shape makeCube(const glm::vec3 position= glm::vec3(0), const float scale=1.0f);
	Shape makePyramid(const glm::vec3 position = glm::vec3(0), const float scale = 1.0f);
	Shape makeSphere(const glm::vec3 position = glm::vec3(0), const float scale = 1.0f);
}