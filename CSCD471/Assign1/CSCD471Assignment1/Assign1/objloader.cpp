#include <iostream>    // std::cout  
#include <fstream>
#include <sstream>
#include <string>
#include <cmath>
#include <string>         // std::string
#include <cstddef>         // std::size_t
#include <GL/glew.h>
#include "objloader.h"

using glm::vec3;
using glm::normalize;
using glm::triangleNormal;
using std::vector;
using std::array;
using std::advance;
using std::copy;
using std::cout;
using std::endl;
using std::string;
using std::tuple;
using std::numeric_limits;
using std::get;
using std::make_tuple;
using std::distance;
using std::max_element;

void OBJLoader:: computeNormals(vector<vec3> const &vertices, vector<int> const &indices, vector<vec3> &normals)
{		
	normals.resize(vertices.size(), vec3(0.0f, 0.0f, 0.0f));
		
	// Compute per-vertex normals here!
	for (size_t i = 0;i < vertices.size();i++)
	{
		vector<vec3> normalsVec = getAdjacentTriangleNormals(i,mVertices,vIndices);
		normals[i] = computeVertexNormal(normalsVec);
		//printf("%llu: %f,%f,%f\n",i,normals[i][0],normals[i][1],normals[i][2]);
		normalsVec.clear();
	}
}

vec3 computeVertexNormal(const vector<vec3> normalsToAvg)
{
	vec3 ret;
	for (auto& norm : normalsToAvg)
	{
		ret += norm;
	}
	return normalize(ret);
}

vector<vec3> getAdjacentTriangleNormals(const size_t ind,
	const std::vector<glm::vec3>& vertices,
	const std::vector<int>& indices)
{
	vector<vec3> ret;
	for (auto beg = indices.begin(), end = indices.end();beg != end;advance(beg, NumPointsPerTriangle))
	{
		//if (contains(beg, beg + NumPointsPerTriangle, ind))
		//considerably faster to do it this way
		if((beg[0]==ind)||(beg[1]==ind)||(beg[2]==ind))
		{
			ret.emplace_back(triangleNormal(vec3(vertices[beg[0]]), vec3(vertices[beg[1]]), vec3(vertices[beg[2]])));
		}
	}
	return ret;
}

void OBJLoader::unitize()
{
	auto maxPos = getMaxXYZ();
	auto minPos = getMinXYZ();
	
	printf("max:\nx: %f, y: %f, z: %f\n",get<0>(maxPos),get<1>(maxPos),get<2>(maxPos));
	printf("min:\nx: %f, y: %f, z: %f\n",get<0>(minPos),get<1>(minPos),get<2>(minPos));
	
	float largestAxis = getLargestAxisValue(maxPos,minPos);
	vec3 offsetFromCenter = getOffsetFromCenter(maxPos,minPos);
	centerObject(offsetFromCenter);
	double scale_factor = 2.0/largestAxis;
	
	printf("scale_factor: %f\n",scale_factor);
	
	scale(scale_factor);
}

void OBJLoader::centerObject(const glm::vec3 offset)
{
	for(auto& vec: mVertices)
	{
		vec-=offset;
	}
}

void OBJLoader::scale(const double scale_factor)
{
	for(auto&vec:mVertices)
	{
		vec*=scale_factor;
	}
}

float OBJLoader::getLargestAxisValue(const std::tuple<float,float,float> maxXYZ,const std::tuple<float,float,float> minXYZ)const
{
	XYZ axis;
	float x = get<0>(maxXYZ)-get<0>(minXYZ);
	float y = get<1>(maxXYZ)-get<1>(minXYZ);
	float z = get<2>(maxXYZ)-get<2>(minXYZ);
	array<float,3> tmp{x,y,z};
	auto result = max_element(tmp.begin(),tmp.end());
	auto dist = distance(tmp.begin(),result);
	if(dist == 0)
	{
		axis = XYZ::X;
	}
	else if(dist == 1)
	{
		axis = XYZ::Y;
	}
	else
	{
		axis = XYZ::Z;
	}
	cout<<"Largest axis is "<<axis<<" with length of "<<*result<<"."<<endl;
	return *result;
}

tuple<float,float,float> OBJLoader::getMaxXYZ()const
{
	float x = -numeric_limits<float>::infinity();
	float y = -numeric_limits<float>::infinity();
	float z = -numeric_limits<float>::infinity();
	
	for(auto& vec:mVertices)
	{
		if(vec[0] > x)
		{
			x = vec[0];
		}
		if(vec[1]>y)
		{
			y = vec[1];
		}
		if(vec[2]>z)
		{
			z = vec[2];
		}
	}
	
	return make_tuple(x,y,z);
}

tuple<float,float,float> OBJLoader::getMinXYZ()const
{
	float x = numeric_limits<float>::infinity();
	float y = numeric_limits<float>::infinity();
	float z = numeric_limits<float>::infinity();
	
	for(auto&vec:mVertices)
	{
		if(vec[0]<x)
		{
			x = vec[0];
		}
		if(vec[1]<y)
		{
			y = vec[1];
		}
		if(vec[2]<z)
		{
			z = vec[2];
		}
	}
	
	return make_tuple(x,y,z);
}

vec3 OBJLoader::getOffsetFromCenter(const std::tuple<float,float,float> maxXYZ, const std::tuple<float,float,float> minXYZ)const
{
	float x = (get<0>(maxXYZ)+get<0>(minXYZ))/2.0;
	float y = (get<1>(maxXYZ)+get<1>(minXYZ))/2.0;
	float z = (get<2>(maxXYZ)+get<2>(minXYZ))/2.0;
	return vec3(x,y,z);
}

OBJLoader::OBJLoader() :
mVertices(0),
mNormals(0),
vIndices(0),
nIndices()
{
	cout << "Called OBJFileReader constructor" << endl;
}

OBJLoader::~OBJLoader()
{
	cout << "Called OBJFileReader destructor" << endl;
}

bool OBJLoader::load(const char *filename)
{
	// Open OBJ file
	std::ifstream OBJFile(filename);
	if (!OBJFile.is_open()) {
		std::cerr << "Could not open " << filename << endl;
		return false;
	}
	
	// Extract vertices and indices
	std::string line;
	vec3 vertex;
	//glm::vec2 uv;
	while (!OBJFile.eof()) {
		getline(OBJFile, line);
		if ((line.find('#') == string::npos) && (line.find('m') == string::npos)){
			if (line.find('v') != string::npos) {

				if ((line.find('t') == string::npos) && (line.find('n') == string::npos))
				{
					std::istringstream vertexLine(line.substr(2));
					vertexLine >> vertex.x;
					vertexLine >> vertex.y;
					vertexLine >> vertex.z;
				    mVertices.emplace_back(vertex);
				}
			}

			else if (line.find("f ") != string::npos) 
			{				
				std::istringstream faceLine(line);
				std::string val1;
				faceLine >> val1;
				int val;
				for (int n = 0; n < 3; n++){
					faceLine >> val;
					
					vIndices.emplace_back(val- 1);
					nIndices.emplace_back(val - 1);

				}
			}
	    }
	}

	// Close OBJ file
	OBJFile.close();

	
	// Compute normals
	computeNormals(mVertices, vIndices, mNormals);
	
	// Display log message
	
	unitize();
	return true;
}

vector<vec3> const &OBJLoader::getVertices() const
{
	return mVertices;
}

vector<vec3> const &OBJLoader::getNormals() const
{
	return mNormals;
}


vector<int> const &OBJLoader::getVertexIndices() const
{
	return vIndices;
}

vector<int> const &OBJLoader::getNormalIndices() const
{
	return nIndices;
}

std::ostream& operator<<(std::ostream& out, XYZ xyz)
{
	switch(xyz)
	{
		case X:
			out<<"X";
			break;
		case Y:
			out<<"Y";
			break;
		case Z:
			out<<"Z";
			break;
	}
	return out;
}
