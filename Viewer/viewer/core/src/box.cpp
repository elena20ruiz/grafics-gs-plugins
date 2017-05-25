#include "box.h"
#include <QtOpenGL/qgl.h>
#include "vector.h"

Box::Box(const Point& point)
    : pmin(point), pmax(point)
{}

Box::Box(const Point& minimum, const Point& maximum)
    : pmin(minimum), pmax(maximum)
{}

void Box::expand(const Point& p)
{
    if (p.x() < pmin.x()) pmin.setX(p.x());
    if (p.y() < pmin.y()) pmin.setY(p.y());
    if (p.z() < pmin.z()) pmin.setZ(p.z());
    if (p.x() > pmax.x()) pmax.setX(p.x());
    if (p.y() > pmax.y()) pmax.setY(p.y());
    if (p.z() > pmax.z()) pmax.setZ(p.z());
}

void Box::expand(const Box& b)
{
    expand(b.pmin);
    expand(b.pmax);
}

/*
void Box::render()
{
    glBegin(GL_LINES);
    glVertex3f(pmin.x(), pmin.y(), pmin.z());
    glVertex3f(pmax.x(), pmin.y(), pmin.z());

    glVertex3f(pmax.x(), pmin.y(), pmin.z());
    glVertex3f(pmax.x(), pmax.y(), pmin.z());

    glVertex3f(pmax.x(), pmax.y(), pmin.z());
    glVertex3f(pmin.x(), pmax.y(), pmin.z());

    glVertex3f(pmin.x(), pmax.y(), pmin.z());
    glVertex3f(pmin.x(), pmin.y(), pmin.z());

    glVertex3f(pmin.x(), pmin.y(), pmax.z());
    glVertex3f(pmax.x(), pmin.y(), pmax.z());

    glVertex3f(pmax.x(), pmin.y(), pmax.z());
    glVertex3f(pmax.x(), pmax.y(), pmax.z());

    glVertex3f(pmax.x(), pmax.y(), pmax.z());
    glVertex3f(pmin.x(), pmax.y(), pmax.z());

    glVertex3f(pmin.x(), pmax.y(), pmax.z());
    glVertex3f(pmin.x(), pmin.y(), pmax.z());

    glVertex3f(pmin.x(), pmax.y(), pmin.z());
    glVertex3f(pmin.x(), pmax.y(), pmax.z());

    glVertex3f(pmax.x(), pmax.y(), pmin.z());
    glVertex3f(pmax.x(), pmax.y(), pmax.z());

    glVertex3f(pmin.x(), pmin.y(), pmin.z());
    glVertex3f(pmin.x(), pmin.y(), pmax.z());

    glVertex3f(pmax.x(), pmin.y(), pmin.z());
    glVertex3f(pmax.x(), pmin.y(), pmax.z());

    glEnd();
}
*/

Point Box::center() const
{
    return 0.5f*(pmax+pmin); 
}

float Box::radius() const
{
    Vector v = pmax - pmin;
    return 0.5f*v.length();
}

Point Box::min() const
{
    return pmin;
}

Point Box::max() const
{
    return pmax;
}
