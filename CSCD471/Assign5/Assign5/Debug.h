#pragma once
#include <string>
#include <cstdlib>
//uncomment to disable debug print statements
//#define NDEBUG
#ifndef NDEBUG
template<typename str,typename...a>
void dprintf(str format,a... args)
{
	printf(format, args...);
}

#else
template<typename str, typename...a>
void dprintf(str format, a... args)
{
	//do nothing
}

#endif

