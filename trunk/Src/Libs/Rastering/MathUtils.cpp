#include "StdAfx.h"

#include "MathUtils.h"

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

		int NextPowerOfTwo(int x)
		{
			double logbase2 = log((float)x) / log((float)2);
			return (int)(pow(2, ceil(logbase2)));
		}
	}
}
