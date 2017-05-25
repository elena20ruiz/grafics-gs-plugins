#ifndef BOX_H
#define BOX_H

#include "point.h"

/*!
	\class Box
	\brief The %Box class represents an axis-aligned box in 3D space (usually a bounding box).  
	
	The Box class represents a box in 3D space through its (min, max) corners.
	This class is suitable for representing axis-aligned boxes (but not oriented boxes).
	A typical use is for representing the bounding box of an object or group of objects.
*/

class CORE_EXPORT Box
{
public:

	/*!
	Constructs a box containing a single point (minimum=maximum=point).
	*/ 
    Box(const Point& point=Point());

	/*!
	Constructs a box from two points (minimum, maximum).
	*/ 
    Box(const Point& minimum, const Point& maximum);

    /*! 
	Expands the box so that it includes point p.
	*/
    void expand(const Point& p);

	/*!
    Expands the box so that it includes box b.
	*/
    void expand(const Box& b);

    /*! 
	Draws the box (wireframe).
	*/
    //void render();

    /*! 
	Returns the center of the box.
	*/
    Point center() const;

    /*! 
	Returns half the box's diagonal, that is, the radius of a sphere bounding the box.
	*/
    float radius() const;

    /*! 
	Returns the minimum corner of the box.
	*/
    Point min() const;

    /*! 
	Returns the maximum corner of the box.
	*/
    Point max() const;


private:
    Point pmin, pmax;	// min, max corners of the box

};

#endif

