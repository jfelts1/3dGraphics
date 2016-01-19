#pragma once

template<class InputIt, class T>
bool contains(InputIt first, InputIt last, const T& value)
{
	for (auto beg = first, end = last;beg != end;beg++)
	{
		if (*beg == value)
		{
			return true;
		}
	}
	return false;
}
