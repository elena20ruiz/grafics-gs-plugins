#ifndef VERTEX_H
#define VERTEX_H

#include "point.h"
#include "vector.h"

/*!
	\class Vertex
	\brief The %Vertex class represents a vertex with a single attribute (vertex coordinates).
*/

class CORE_EXPORT Vertex
{
public:
    /*!
    Constructs a vertex with the given (x,y,z) coordinates.
    */
    Vertex(const Point& coords);

    /*!
    Returns the (x,y,z) coordinates of the vertex.
    */
    Point coord() const;              

    /*!
    Sets the (x,y,z) coordinates of the vertex.
    */
    void setCoord(const Point& coord);

private:
    Point pcoord;		// vertex coordinates
};

#endif
