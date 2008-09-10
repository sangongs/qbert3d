#pragma once

namespace BGComplete
{
	namespace Math
	{
		float DSin(float degrees);
		float DCos(float degrees);
		float Dacos(float value);
		float Modulu(float inp);
		int Round(float inp);
		int NextPowerOfTwo(int x);
		float Swing(unsigned precision); //every second goes from 0.0f - > 1.0f -> 0.0f at 1/precision precision.
	}
}
