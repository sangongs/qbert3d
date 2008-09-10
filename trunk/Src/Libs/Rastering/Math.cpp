#include "StdAfx.h"

#include "Math.h"

namespace BGComplete
{

namespace Math
{
	float DSin(float degrees)
	{
		return (float)sin(degrees * (M_PI / 180));
	}

	float DCos(float degrees)
	{
		return (float)cos(degrees * (M_PI / 180));
	}

	float Dacos(float value)
	{
		return (float)(acos(value) * (180 / M_PI));
	}

	float Modulu(float inp)
	{
		return inp - floor(inp);
	}

	int Round(float inp)
	{
		return (int)(floor(inp) + ((Modulu(inp) < 0.5) ? 0 : 1));
	}

}	//namespace Math

}	//namespace BGComplete
