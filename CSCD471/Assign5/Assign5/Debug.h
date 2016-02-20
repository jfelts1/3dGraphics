#pragma once
//uncomment to disable debug print statements and asserts
#define NDEBUG
#include <string>
#include <cstdlib>
#include <cassert>
#ifdef _WIN32
#pragma warning(push)
#pragma warning(disable: 4100)
#endif
#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#endif
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
#ifdef _WIN32
#pragma warning(pop)
#endif
#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif
