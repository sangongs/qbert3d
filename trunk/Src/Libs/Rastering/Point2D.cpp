#include "StdAfx.h"
#include "Point2D.h"

namespace BGComplete {
	namespace Math {

Point2D::Point2D(const std::string& arguments)
{
	if (sscanf(arguments.c_str(), "%f %f", Points, Points + 1) != 2)
		throw std::exception("Couldn't read line into Point2D");
}

}
}