#include "MinMax.h"

template<class T> T Minimum(T a, T b, T c)
{
	if (a < b)
	{	
		if (a < c)
		{
			return a;
		}
	}
	else if (b < c)
	{
		return b;
	}
	return c;
}
template<class T> T Maximum(T a, T b, T c)
{
	if (a > b)
	{	
		if (a > c)
		{
			return a;
		}
	}
	else if (b > c)
	{
		return b;
	}
	return c;
}