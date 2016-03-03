#include "Shapes.h"
#include <cmath>
#include <algorithm>
using glm::vec3;
using std::vector;
using std::array;
using std::sin;
using std::cos;
using std::max_element;
using glm::vec2;

#define CUBE_VERTICES vector<vec3>{\
	vec3(1.0f, 1.0f, 1.0f),  /* v0,v1,v2,v3 (front)*/\
	vec3(-1.0f, 1.0f, 1.0f),\
	vec3(-1.0f, -1.0f, 1.0f),\
	vec3(1.0f, -1.0f, 1.0f),\
	vec3(1.0f, 1.0f, 1.0f),  /* v0,v3,v4,v5 (right)*/\
	vec3(1.0f, -1.0f, 1.0f),\
	vec3(1.0f, -1.0f, -1.0f),\
	vec3(1.0f, 1.0f, -1.0f),\
	vec3(1.0f, 1.0f, 1.0f),  /* v0,v5,v6,v1 (top)*/\
	vec3(1.0f, 1.0f, -1.0),\
	vec3(-1.0f, 1.0f, -1.0f),\
	vec3(-1.0f, 1.0f, 1.0f),\
	vec3(-1.0f, 1.0f, 1.0f), /* v1,v6,v7,v2 (left)*/\
	vec3(-1.0f, 1.0f, -1.0f),\
	vec3(-1.0f, -1.0f, -1.0f),\
	vec3(-1.0f, -1.0f, 1.0f),\
	vec3(-1.0f, -1.0f, -1.0f),/* v7,v4,v3,v2 (bottom)*/\
	vec3(1.0f, -1.0f, -1.0f),\
	vec3(1.0f, -1.0f, 1.0f),\
	vec3(-1.0f, -1.0f, 1.0f),\
	vec3(1.0f, -1.0f, -1.0f),/* v4,v7,v6,v5 (back)*/\
	vec3(-1.0f, -1.0f, -1.0f),\
	vec3(-1.0f, 1.0f, -1.0f),\
	vec3(1.0f, 1.0f, -1.0f),\
}

#define CUBE_NORMALS vector<vec3>{\
	vec3(0.0f, 0.0f, 1.0f), /* v0,v1,v2,v3 (front)*/\
	vec3(0.0f, 0.0f, 1.0f),\
	vec3(0.0f, 0.0f, 1.0f),\
	vec3(0.0f, 0.0f, 1.0f),\
	vec3(1.0f, 0.0f, 0.0f), /* v0,v3,v4,v5 (right)*/\
	vec3(1.0f, 0.0f, 0.0f),\
	vec3(1.0f, 0.0f, 0.0f),\
	vec3(1.0f, 0.0f, 0.0f),\
	vec3(0.0f, 1.0f, 0.0f), /* v0,v5,v6,v1 (top)*/\
	vec3(0.0f, 1.0f, 0.0f),\
	vec3(0.0f, 1.0f, 0.0f),\
	vec3(0.0f, 1.0f, 0.0f),\
	vec3(-1.0f, 0.0f, 0.0f),/* v1,v6,v7,v2 (left)*/\
	vec3(-1.0f, 0.0f, 0.0f),\
	vec3(-1.0f, 0.0f, 0.0f),\
	vec3(-1.0f, 0.0f, 0.0f),\
	vec3(0.0f, -1.0f, 0.0f), /* v7,v4,v3,v2 (bottom)*/\
	vec3(0.0f, -1.0f, 0.0f),\
	vec3(0.0f, -1.0f, 0.0f),\
	vec3(0.0f, -1.0f, 0.0f),\
	vec3(0.0f, 0.0f, -1.0f), /* v4,v7,v6,v5 (back)*/\
	vec3(0.0f, 0.0f, -1.0f),\
	vec3(0.0f, 0.0f, -1.0f),\
	vec3(0.0f, 0.0f, -1.0f)\
}

#define CUBE_INDICES vector<GLuint>{\
	0, 1, 2, 2, 3, 0,            /* front*/\
	4, 5, 6, 6, 7, 4,            /* right*/\
	8, 9, 10, 10, 11, 8,         /* top*/\
	12, 13, 14, 14, 15, 12,      /* left*/\
	16, 17, 18, 18, 19, 16,      /* bottom*/\
	20, 21, 22, 22, 23, 20		 /* back*/\
}

#define CUBE_TEXTURES vector<vec2> {\
	vec2(0.0, 0.0),      /*front*/\
	vec2(2.0, 0.0),      \
    vec2(2.0f, 2.0f),    \
	vec2(0.0, 2.0),      \
	vec2(0.0, 0.0),      /*right*/\
	vec2(2.0, 0.0),      \
    vec2(2.0f, 2.0f),    \
	vec2(0.0, 2.0),      \
	vec2(0.0, 0.0),      /*top*/\
	vec2(2.0, 0.0),      \
    vec2(2.0f, 2.0f),    \
	vec2(0.0, 2.0),      \
	vec2(0.0, 0.0),      /*left*/\
	vec2(2.0, 0.0),      \
    vec2(2.0f, 2.0f),    \
	vec2(0.0, 2.0),      \
	vec2(0.0, 0.0),      /*bottom*/\
	vec2(2.0, 0.0),      \
    vec2(2.0f, 2.0f),    \
	vec2(0.0, 2.0),      \
	vec2(0.0, 0.0),      /*back*/\
	vec2(2.0, 0.0),      \
    vec2(2.0f, 2.0f),    \
	vec2(0.0, 2.0),      \
}

Shape Shapes::makeCube(const vec3 position, const float scale)
{
	return Shape(CUBE_VERTICES, CUBE_NORMALS, CUBE_TEXTURES, CUBE_INDICES, position, scale);
}

