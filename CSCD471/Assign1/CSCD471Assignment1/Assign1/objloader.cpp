#include <iostream>    // std::cout  
#include <fstream>
#include <sstream>
#include <string>
#include <cmath>
#include <string>         // std::string
#include <cstddef>         // std::size_t
#include "objloader.h"


void OBJLoader:: computeNormals(std::vector<glm::vec3> const &vertices, std::vector<int> const &indices, std::vector<glm::vec3> &normals){
		
	    normals.resize(vertices.size(), glm::vec3(0.0f, 0.0f, 0.0f));
		
		// Compute per-vertex normals here!

}


OBJLoader::OBJLoader() :
mVertices(0),
mNormals(0),
vIndices(0),
nIndices()
{
	std::cout << "Called OBJFileReader constructor" << std::endl;
}

OBJLoader::~OBJLoader()
{
	std::cout << "Called OBJFileReader destructor" << std::endl;
}

bool OBJLoader::load(const char *filename)
{
	// Open OBJ file
	std::ifstream OBJFile(filename);
	if (!OBJFile.is_open()) {
		std::cerr << "Could not open " << filename << std::endl;
		return false;
	}
	
	// Extract vertices and indices
	std::string line;
	glm::vec3 vertex;
	glm::vec2 uv;
	while (!OBJFile.eof()) {
		getline(OBJFile, line);
		if ((line.find('#') == -1) && (line.find('m') == -1)){
			if (line.find('v') != -1) {

				if ((line.find('t') == -1) && (line.find('n') == -1)){
					std::istringstream vertexLine(line.substr(2));
					vertexLine >> vertex.x;
					vertexLine >> vertex.y;
					vertexLine >> vertex.z;
				    mVertices.push_back(vertex);
				}
			}

			else if (line.find("f ") != -1) {
				std::istringstream faceLine(line);
				std::string val1;
				faceLine >> val1;
				int val;
				for (int n = 0; n < 3; n++){
					faceLine >> val;
					
					vIndices.push_back(val- 1);
					nIndices.push_back(val - 1);

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

std::vector<glm::vec3> const &OBJLoader::getVertices() const
{
	return mVertices;
}

std::vector<glm::vec3> const &OBJLoader::getNormals() const
{
	return mNormals;
}


std::vector<int> const &OBJLoader::getVertexIndices() const
{
	return vIndices;
}

std::vector<int> const &OBJLoader::getNormalIndices() const
{
	return nIndices;
}
