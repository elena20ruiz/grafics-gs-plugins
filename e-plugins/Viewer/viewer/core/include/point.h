#ifndef POINT_H
#define POINT_H

#include "core_global.h"
#include <QVector3D>
#include <iostream>

/*!
	\class Point
	\brief The %Point class represents a point (x,y,z) in 3D space.
	
	For the sake of simplicity, Point is implemented as a typedef of Qt's 
	<a href="http://qt-project.org/doc/qt-4.8/qvector3d.html"> QVector3D. </a>    

	Example of use:
    @code
        #include "point.h"
        #include "vector.h"

        // create two points
        Point p = Point(0.0, 0.0, 0.0);
        Point q = Point(1.0, 0.0, 0.0);
	
	    // get coordinate values
	    float x = p.x(); 
	    float y = p.y(); 
	    float z = p.z();

	    // set coordinate values
	    p.setX(2.0);
	    p.setY(4.0);
	    p.setZ(2.0);

        // common operations
	    Vector v = p - q; // point substraction (returns a Vector)
	    Point r = 0.4*p + 0.6*q; // barycentric combination
    @endcode
		
*/

typedef QVector3D Point;

std::ostream& operator<< (std::ostream &os, const Point &p);

#endif
