/*
James Felts 2015
*/
#ifndef FILEUTILS_H
#define FILEUTILS_H
#ifdef _MSC_VER
#pragma warning(disable: 4505 4514 4668 4820 4710 4711)
#endif
#include <string>
#include <iostream>
#include <fstream>
#include <stdexcept>
namespace Utils
{
	//returns the entire file as a string
	std::string readFileAsText(std::string filepath);
}
#endif