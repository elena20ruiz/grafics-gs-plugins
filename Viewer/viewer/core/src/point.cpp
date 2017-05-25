#include "point.h"

std::ostream& operator<< (std::ostream &os, const Point &v) 
{
	os << v.x() << ", " << v.y() << ", " << v.z() << std::endl;
	return os;
}


