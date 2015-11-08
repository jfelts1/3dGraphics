/*
James Felts 2015
*/
#include "FileUtils.h"

using std::string;
using std::ifstream;

string Utils::readFileAsText(string filepath)
{
	string rawString;
	string tmp;
	ifstream fin;
	fin.open(filepath.c_str(), fin.in);
	if (fin.is_open())
	{
		getline(fin, tmp);
		rawString.append(tmp);
		rawString.push_back('\n');
		while (!fin.eof())
		{
			getline(fin, tmp);
			rawString.append(tmp);
			rawString.push_back('\n');
		}

		fin.close();
	}
	else
	{
		throw std::runtime_error("File not found:" + filepath);
	}
	return rawString;
}
