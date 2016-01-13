#include <iostream>    // std::cout  
#include <fstream>
#include <sstream>
#include <string>
#include <cmath>
#include <string>         // std::string
#include <cstddef>         // std::size_t
#include "objloader.h"
#include <GL/glew.h>

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

void OBJLoader:: computeNormals(vector<vec3> const &vertices, vector<int> const &indices, vector<vec3> &normals)
{		
	normals.resize(vertices.size(), vec3(0.0f, 0.0f, 0.0f));
		
	// Compute per-vertex normals here!
	for (size_t i = 0;i < vertices.size();i++)
	{
		vector<vec3> normalsVec = getAdjacentTriangleNormals(i);
		normals[i] = computeVertexNormal(normalsVec);
		//printf("%llu: %f,%f,%f\n",i,normals[i][0],normals[i][1],normals[i][2]);
		normalsVec.clear();
	}
}

vec3 OBJLoader::computeVertexNormal(const vector<vec3> normalsVec) const
{
	vec3 ret;
	for (auto& norm : normalsVec)
	{
		ret += norm;
	}
	return normalize(ret);
}

vector<vec3> OBJLoader::getAdjacentTriangleNormals(const size_t ind)
{
	vector<vec3> ret;
	for (auto beg = vIndices.begin(), end = vIndices.end();beg != end;advance(beg, NumPointsPerTriangle))
	{
		//if (contains(beg, beg + NumPointsPerTriangle, ind))
		//considerably faster to do it this way
		if((beg[0]==ind)||(beg[1]==ind)||(beg[2]==ind))
		{
			ret.emplace_back(triangleNormal(vec3(mVertices[beg[0]]), vec3(mVertices[beg[1]]), vec3(mVertices[beg[2]])));
		}
	}
	return ret;
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
