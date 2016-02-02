#include "Shapes.h"
using glm::vec3;

#define CUBE_VERTICES std::vector<vec3>{\
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

#define CUBE_NORMALS std::vector<vec3>{\
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

#define CUBE_INDICES std::vector<GLuint>{\
	0, 1, 2, 2, 3, 0,            /* front*/\
	4, 5, 6, 6, 7, 4,            /* right*/\
	8, 9, 10, 10, 11, 8,         /* top*/\
	12, 13, 14, 14, 15, 12,      /* left*/\
	16, 17, 18, 18, 19, 16,      /* bottom*/\
	20, 21, 22, 22, 23, 20		 /* back*/\
}

#define CUBE_TEXTURES std::vector<float> {\
	0.0, 0.0,      /*front*/\
	2.0, 0.0,      \
	2.0f, 2.0f,   \
	0.0, 2.0,      \
	0.0, 0.0,      /*right*/\
	2.0, 0.0,      \
	2.0f, 2.0f,   \
	0.0, 2.0,      \
	0.0, 0.0,      /*top*/\
	2.0, 0.0,      \
	2.0f, 2.0f,   \
	0.0, 2.0,      \
	0.0, 0.0,      /*left*/\
	2.0, 0.0,      \
	2.0f, 2.0f,   \
	0.0, 2.0,      \
	0.0, 0.0,      /*bottom*/\
	2.0, 0.0,      \
	2.0f, 2.0f,   \
	0.0, 2.0,      \
	0.0, 0.0,      /*back*/\
	2.0, 0.0,      \
	2.0f, 2.0f,   \
	0.0, 2.0,      \
}


#define PYRAMID_VERTICES std::vector<vec3>{\
        vec3(-1.0f, -1.0f, -1.0f),	/*v0*/\
		vec3(1.0f, -1.0f, -1.0f),	/*v1*/\
		vec3(1.0f, -1.0f, 1.0f),	/*v2*/\
        vec3(-1.0f, -1.0f, 1.0f),	/*v3*/\
		vec3(0.0f,1.0f,0.0f),		/*v4*/\
    }

#define PYRAMID_NORMALS std::vector<vec3>{\
        vec3(0.0f, 0.0f, 1.0f),\
        vec3(0.0f, 0.0f, 1.0f),\
        vec3(0.0f, 0.0f, 1.0f),\
        vec3(0.0f, 0.0f, 1.0f),\
		vec3(0.0f, 0.0f, 1.0f),\
	}

#define PYRAMID_INDICES std::vector<GLuint>{\
		/*base of pyramid*/\
        0, 1, 2,\
		0, 2, 3,\
		/*sides of pyramid*/\
		0,1,4,\
		1,2,4,\
		2,3,4,\
		3,0,4\
}

#define PYRAMID_TEXTURES std::vector<float>{\
        0.0, 0.0, \
        2.0, 0.0,    \
        2.0f, 2.0f,  \
        0.0, 2.0, \
    }


Shape Shapes::makeCube(const vec3 position, const float scale)
{
	return Shape(CUBE_VERTICES, CUBE_NORMALS, CUBE_TEXTURES, CUBE_INDICES, position, scale);
}

Shape Shapes::makePyramid(const vec3 position, const float scale)
{
	return Shape(PYRAMID_VERTICES,PYRAMID_NORMALS,PYRAMID_TEXTURES,PYRAMID_INDICES,position,scale);
}

Shape Shapes::makeSphere(const vec3 position, const float scale)
{
	return Shape();
}