#pragma once

namespace BGComplete {
	namespace Math
{
	class Point2D
	{
	public:
		Point2D(float x, float y);
		Point2D(const std::string& argument);
		Point2D() {}
		float Points[2];
	};
}
}