#include "face.h"
#include <cmath>

Face::Face() 
{}

Face::Face(int i0, int i1, int i2, int i3)
{
    pvertices.push_back(i0);
    pvertices.push_back(i1);
    pvertices.push_back(i2);
    if (i3!=-1) pvertices.push_back(i3);
}

void Face::computeNormal(const vector<Vertex> &v)
{
    int j;
    pnormal=Vector(0.0f, 0.0f, 0.0f);
    int n = pvertices.size();
    for (int i=0; i<n; i++)
    {
        j = (i+1)%n;
        const Point& pi = v[pvertices[i]].coord();
        const Point& pj = v[pvertices[j]].coord();

        pnormal.setX(pnormal.x() + ((pi.z() + pj.z()) * (pi.y() - pj.y())));
        pnormal.setY(pnormal.y() + ((pi.x() + pj.x()) * (pi.z() - pj.z())));
        pnormal.setZ(pnormal.z() + ((pi.y() + pj.y()) * (pi.x() - pj.x())));
    }
    pnormal.normalize();
}


int Face::numVertices() const
{
    return pvertices.size();
}

int Face::vertexIndex(int i) const
{
    return pvertices[i];
}

void Face::addVertexIndex(int i)
{
    pvertices.push_back(i);
}

Vector Face::normal() const
{
    return pnormal;
}
