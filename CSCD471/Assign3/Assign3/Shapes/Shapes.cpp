#include "Shapes.h"
#include <cmath>
#include <algorithm>
using glm::vec3;
using std::vector;
using std::array;
using std::sin;
using std::cos;
using std::max_element;

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

#define CUBE_TEXTURES vector<float> {\
	0.0, 0.0,      /*front*/\
	2.0, 0.0,      \
    2.0f, 2.0f,    \
	0.0, 2.0,      \
	0.0, 0.0,      /*right*/\
	2.0, 0.0,      \
    2.0f, 2.0f,    \
	0.0, 2.0,      \
	0.0, 0.0,      /*top*/\
	2.0, 0.0,      \
    2.0f, 2.0f,    \
	0.0, 2.0,      \
	0.0, 0.0,      /*left*/\
	2.0, 0.0,      \
    2.0f, 2.0f,    \
	0.0, 2.0,      \
	0.0, 0.0,      /*bottom*/\
	2.0, 0.0,      \
    2.0f, 2.0f,    \
	0.0, 2.0,      \
	0.0, 0.0,      /*back*/\
	2.0, 0.0,      \
    2.0f, 2.0f,    \
	0.0, 2.0,      \
}


#define PYRAMID_VERTICES vector<vec3>{\
        vec3(-1.0f, -1.0f, -1.0f),	/*v0*/\
		vec3(1.0f, -1.0f, -1.0f),	/*v1*/\
		vec3(1.0f, -1.0f, 1.0f),	/*v2*/\
        vec3(-1.0f, -1.0f, 1.0f),	/*v3*/\
		vec3(0.0f,1.0f,0.0f),		/*v4*/\
		/*vec3(-1.0f, -1.0f, -1.0f),	/*v0 5*/\
		/*vec3(1.0f, -1.0f, -1.0f),	/*v1 6*/\
		/*vec3(1.0f, -1.0f, 1.0f),	/*v2 7*/\
        /*vec3(-1.0f, -1.0f, 1.0f),	/*v3 8*/\
		/*vec3(0.0f,1.0f,0.0f),		/*v4 9*/\
		/*vec3(0.0f,1.0f,0.0f),		/*v4 10*/\
		/*vec3(0.0f,1.0f,0.0f),		/*v4 11*/\
    }

#define PYRAMID_NORMALS vector<vec3>{\
        vec3(0.0f, 0.0f, 1.0f),\
        vec3(0.0f, 0.0f, 1.0f),\
        vec3(0.0f, 0.0f, 1.0f),\
        vec3(0.0f, 0.0f, 1.0f),\
		vec3(0.0f, 0.0f, 1.0f),\
	}

#define PYRAMID_INDICES vector<GLuint>{\
		/*base of pyramid*/\
        0, 1, 2,\
        0, 2, 3,\
		/*sides of pyramid*/\
        0,1,4,\
        1,2,4,\
        2,3,4,\
        3,0,4 \
}

#define PYRAMID_TEXTURES vector<float>{\
        1.0, 0.0,   /*Bottom*/\
        1.0, 1.0,    \
        0.0f, 0.0f,  \
        0.0, 1.0,    \
        0.0, 0.0,   /*side*/\
        1.0, 0.0,    \
        0.5f, 1.0f,  \
        0.0, 0.0,   /*side*/\
        1.0, 0.0,    \
        0.5f, 1.0f,  \
        0.0, 0.0,   /*side*/\
        1.0, 0.0,    \
        0.5f, 1.0f,  \
        0.0, 0.0,   /*side*/\
        1.0, 0.0,    \
        0.5f, 1.0f,  \
    }


Shape Shapes::makeCube(const vec3 position, const float scale)
{
	return Shape(CUBE_VERTICES, CUBE_NORMALS, CUBE_TEXTURES, CUBE_INDICES, position, scale);
}

Shape Shapes::makePyramid(const vec3 position, const float scale)
{
	return Shape(PYRAMID_VERTICES, PYRAMID_NORMALS, PYRAMID_TEXTURES, PYRAMID_INDICES, position, scale);
}

Shape Shapes::makeSphere(const vec3 position, const float scale)
{
	vector<vec3> vertices;
	vector<vec3> normals{vec3(0.0,0.0,0.0)};
	vector<GLuint> indices;
	vector<float> textures{0.0,0.0};
	auto sphereMiddle = Sphere::calculatePoints();
	Sphere::constructSphere(sphereMiddle, vertices, indices);

	return Shape(vertices, normals, textures, indices, position, scale);
}

array<vector<vec3>, NUM_HORIZONTAL_SLICES_SPHERE> Shapes::Sphere::calculatePoints()
{
	constexpr float sliceDiff = static_cast<float>(PI) / static_cast<float>(NUM_HORIZONTAL_SLICES_SPHERE);
	constexpr float vertSliceDiff = static_cast<float>(TWOPI) / static_cast<float>(NUM_VERTICAL_SLICES_SPHERE);
	printf("sliceDiff:%f\nvertSlideDiff:%f\n", sliceDiff, vertSliceDiff);
	float x = 0.0f;
	float z = 0.0f;
	float y = 0.0f;
	float phi = static_cast<float>(PI) / 4.0f;
	float theta;
	array<vector<vec3>, NUM_HORIZONTAL_SLICES_SPHERE> horizontalCircles;
	//move down the sphere
	for (size_t i = 0;i < NUM_HORIZONTAL_SLICES_SPHERE;i++)
	{
		phi -= sliceDiff;
		vector<vec3> curCircle;
		theta = 0.0f;
		//move around the vertical axis calculating points
		for (size_t j = 0;j < NUM_VERTICAL_SLICES_SPHERE;j++)
		{
			x = cos(theta)*sin(phi);
			y = cos(phi);
			z = sin(theta)*sin(phi);
			curCircle.emplace_back(vec3(x, y, z));
			theta += vertSliceDiff;
		}
		horizontalCircles[i] = move(curCircle);
	}
	return horizontalCircles;
}

void Shapes::Sphere::constructSphere(const std::array<vector<vec3>, NUM_HORIZONTAL_SLICES_SPHERE>& sphereMiddle,
	vector<vec3>& vertices,
	vector<GLuint>& indices)
{
	//add top to sphere
	vertices.emplace_back(vec3(0.0f, SPHERE_TOP_Y, 0.0f));
	int vertexOffset = 0;
	//special case for first circle to properly connect to top point
	for (size_t i = 0, j = 0;i < sphereMiddle[0].size() * 2;i += 2, j++)
	{
		vertices.emplace_back(sphereMiddle[0][j]);
		vertexOffset++;
		if (j + 1 < sphereMiddle[0].size())
		{
			vertices.emplace_back(sphereMiddle[0][j+1]);
			vertexOffset++;
		}
        if(i < sphereMiddle[0].size()*2-2)
        {
	        indices.emplace_back(0);
	        indices.emplace_back((i+1)-j);
	        indices.emplace_back((i+2)-j);
        }
        else
        {
	        indices.emplace_back(0);
	        indices.emplace_back((i+1)-j);
	        indices.emplace_back(1);
        }
		
    }
	
    for(size_t i = 1;i<sphereMiddle.size();i++)
    {
        for(size_t k = 0,j=0,l=0;k<sphereMiddle[i].size()*4;k+=4,j+=2,l++)
        {
            vertices.emplace_back(sphereMiddle[i][l]);
			vertexOffset++;
			if(l+1<sphereMiddle[i].size())
			{
				vertices.emplace_back(sphereMiddle[i][l+1]);		
				vertexOffset++;
			}
			if (k < sphereMiddle[i].size() * 4 - 4)
            {
	            indices.emplace_back(i*NUM_HORIZONTAL_SLICES_SPHERE - NUM_HORIZONTAL_SLICES_SPHERE + j + 1);
	            indices.emplace_back(i*NUM_HORIZONTAL_SLICES_SPHERE + (k + 1) - j);
	            indices.emplace_back(i*NUM_HORIZONTAL_SLICES_SPHERE + (k + 2) - j);
	            indices.emplace_back(i*NUM_HORIZONTAL_SLICES_SPHERE + (k + 2) - j);
	            indices.emplace_back(i*NUM_HORIZONTAL_SLICES_SPHERE + (k + 3) - j);
	            indices.emplace_back(i*NUM_HORIZONTAL_SLICES_SPHERE - NUM_HORIZONTAL_SLICES_SPHERE + j + 1);
            }
            else
            {
	            indices.emplace_back(i*NUM_HORIZONTAL_SLICES_SPHERE - NUM_HORIZONTAL_SLICES_SPHERE + j + 1);
	            indices.emplace_back(i*NUM_HORIZONTAL_SLICES_SPHERE + (k + 1) - j);
	            indices.emplace_back(i*NUM_HORIZONTAL_SLICES_SPHERE + 1);
	            indices.emplace_back(i*NUM_HORIZONTAL_SLICES_SPHERE - NUM_HORIZONTAL_SLICES_SPHERE + j + 1);
	            indices.emplace_back(i*NUM_HORIZONTAL_SLICES_SPHERE + (k + 2) - j);
	            indices.emplace_back(i*NUM_HORIZONTAL_SLICES_SPHERE - NUM_HORIZONTAL_SLICES_SPHERE + j + 1);
            }
        }
    }

	//add bottom to sphere
	vertices.emplace_back(vec3(0.0f, -SPHERE_TOP_Y, 0.0f));
	auto bottomPointIndex = vertices.size();
	//special case for the last circle to properly to attcash the bottom point.
	for (size_t i = 0, j = 0;i < (sphereMiddle.end()-1)->size() * 2;i += 2, j++)
	{

		vertices.emplace_back(sphereMiddle[NUM_HORIZONTAL_SLICES_SPHERE-1][j]);
		if (j + 1 < (sphereMiddle.end()-1)->size())
		{
			vertices.emplace_back(sphereMiddle[NUM_HORIZONTAL_SLICES_SPHERE-1][j + 1]);
		}
		if (i < (sphereMiddle.end()-1)->size() * 2 - 2)
		{
			indices.emplace_back(bottomPointIndex);
			indices.emplace_back(vertexOffset+(i + 1) - j);
			indices.emplace_back(vertexOffset+(i + 2) - j);
		}
		else
		{
			indices.emplace_back(bottomPointIndex);
			indices.emplace_back(vertexOffset+(i + 1) - j);
			indices.emplace_back(vertexOffset+1);
		}
	}
}
