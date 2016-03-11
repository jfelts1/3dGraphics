#include <iostream>
#include <fstream>
#include <sstream>
#include <array>
#include <cstdlib>
#include "objloader.h"
using std::string;
using std::vector;
using glm::vec3;
using std::stringstream;
using std::ifstream;
using std::array;
using glm::vec2;
using std::strtof;

objLoader::objLoader(const string& filepath)
{
	ifstream fin;
	fin.open(filepath, std::ios::in);
	if(fin.is_open())
	{	
		fin.seekg(0, fin.end);
		size_t len = fin.tellg();
		fin.seekg(0, fin.beg);
		m_data.resize(len, ' ');
		char* begin = &*m_data.begin();
		fin.read(begin, len);
		fin.close();
	}
	else
	{
		fputs("Failed to open input file",stderr);
	}
}

vector<vec3> objLoader::getVertices()const
{   
	vector<vec3> ret;
	auto vertStr = "v ";
	auto vertStrLen = string(vertStr).size();
	auto pos = m_data.find(vertStr);
	auto pos2 = m_data.find("\n", pos);
	float x, y, z;
	while (pos != string::npos && pos2 != string::npos)
	{
		string tmp = m_data.substr(pos + vertStrLen, pos2 - pos - vertStrLen);
		char* end;
		const char* str = tmp.c_str();
		x = strtof(str,&end);
		str = end;
		y = strtof(str, &end);
		str = end;
		z = strtof(str, &end);
		//printf("[VERT] x:%f, y:%f, z:%f\n",x,y,z);
		ret.emplace_back(vec3(x, y, z));
		pos = m_data.find(vertStr, pos + 1);
		pos2 = m_data.find("\n", pos);
	}
    return ret;
}

vector<vec3> objLoader::getNormals()const
{
	vector<vec3> ret;
	auto normStr = "vn ";
	auto normStrLen = string(normStr).size();
	auto pos = m_data.find(normStr);
	auto pos2 = m_data.find("\n", pos);
	float x, y, z;
	while (pos != string::npos && pos2 != string::npos)
	{
		string tmp = m_data.substr(pos + normStrLen, pos2 - pos - normStrLen);
		char * end;
		const char* str = tmp.c_str();
		x = strtof(str, &end);
		str = end;
		y = strtof(str, &end);
		str = end;
		z = strtof(str, &end);
		//printf("[NORM] x:%f, y:%f, z:%f\n", x, y, z);
		ret.emplace_back(vec3(x, y, z));
		pos = m_data.find(normStr, pos + 1);
		pos2 = m_data.find("\n", pos);
	}
    return ret;
}

vector<vec2> objLoader::getTextures()const
{
    vector<vec2> ret;
	auto texStr = "vt ";
	auto texStrLen = string(texStr).size();
	auto pos = m_data.find(texStr);
	auto pos2 = m_data.find("\n",pos);
	float x, y;
	while(pos != string::npos && pos2 != string::npos)
	{
		string tmp = m_data.substr(pos + texStrLen, pos2 - pos - texStrLen);
		char* end;
		const char* str = tmp.c_str();
		x = strtof(str, &end);
		str = end;
		y = strtof(str, &end);
        //printf("[TEX] x:%f, y:%f\n", x, y);
		ret.emplace_back(vec2(x,y));
		pos = m_data.find(texStr, pos + 1);
		pos2 = m_data.find("\n", pos);
	}
    return ret;
}

vector<GLuint> objLoader::getIndices()const
{
    vector<GLuint> ret;
	auto indStr = "f ";
	auto indStrLen = string(indStr).size();
	auto pos = m_data.find(indStr);
	auto pos2 = m_data.find("\n", pos);
	GLuint x, y, z;
	array<string, 3> temp;
	const int base = 10;
	while (pos != string::npos && pos2 != string::npos)
	{
		stringstream tmp(m_data.substr(pos + indStrLen, pos2 - pos - indStrLen));
		tmp >> temp[0];
		tmp >> temp[1];
		tmp >> temp[2];
		char * end;
		const char* str = temp[0].c_str();
		x = strtol(str, &end,base);
		str = temp[1].c_str();
		y = strtol(str, &end,base);
		str = temp[2].c_str();
		z = strtol(str, &end,base);
		x--;
		y--;
		z--;
		//printf("[IND] x:%i, y:%i, z:%i\n", x, y, z);
		ret.emplace_back(x);
		ret.emplace_back(y);
		ret.emplace_back(z);
		pos = m_data.find(indStr, pos + 1);
		pos2 = m_data.find("\n", pos);
	}
    return ret;
}
