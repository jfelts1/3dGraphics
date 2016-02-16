#pragma once
//uncomment to disable debug print statements
//#define NDEBUG
#include <string>
#include <cstdlib>
#include <cassert>
#ifndef NDEBUG
template<typename...a>
void dprintf(const char* format,a... args)
{
	printf(format, args...);
}

#else
template<typename...a>
void dprintf(const char* format, a... args)
{
	//do nothing
}

#endif

