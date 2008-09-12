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

		int Round(float inp)
		{
			return (int)(floor(inp) + ((Modulu(inp) < 0.5) ? 0 : 1));
		}

		int NextPowerOfTwo(int x)
		{
			double logbase2 = log((float)x) / log((float)2);
			return (int)(pow(2, ceil(logbase2)));
		}
		
		float Swing(unsigned precision)
		{
			float stage = (float)(GetTickCount() % 1000);
			if (stage > precision / 2)
				stage = (float)precision - stage;
			return stage / (float)(precision / 2);
		}

		float Sign(float inp)
		{
			return ((inp >= 0) ? 1.0f : -1.0f);
		}
	}
}
