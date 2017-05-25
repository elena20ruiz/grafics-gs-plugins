#include "vertex.h"

Vertex::Vertex(const Point& coord)
    : pcoord(coord)
{}

Point Vertex::coord() const
{
    return pcoord;
}

void Vertex::setCoord(const Point& coord)
{
    pcoord = coord;
}


