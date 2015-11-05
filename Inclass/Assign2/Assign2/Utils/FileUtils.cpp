/*
James Felts 2015
*/
#include "FileUtils.h"

using std::string;
using std::ifstream;

std::string Utils::readFileAsText(std::string filepath)
{
	string rawString;
	string tmp;
	ifstream fin;
	fin.open(filepath.c_str(), fin.in);
	std::getline(fin, tmp);
	rawString.append(tmp);
	rawString.push_back('\n');
	while (!fin.eof())
	{
		std::getline(fin, tmp);
		rawString.append(tmp);
		rawString.push_back('\n');
	}

	fin.close();
	return rawString;
}
