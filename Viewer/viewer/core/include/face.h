#ifndef FACE_H
#define FACE_H

#include <vector>
#include "vertex.h"

using namespace std;

/*!
	\class Face
	\brief The %Face class represents a face of a 3D object. 

    Each face keeps a list of vertices (actually vertex indices) and a normal vector.
    The face is assumed to be convex. 
*/

class CORE_EXPORT Face
{
public:
    /*! 
    Constructs an empty face.
    */
    Face();

    /*!
    Constructs a face with 3 or 4 vertices (actually vertex indices). 
    */
    Face(int i0, int i1, int i2, int i3=-1); 

    /*!
    Adds a new vertex index i to the face (at the end, i.e. push_back).
    */ 
    void addVertexIndex(int i);       

    /*!
    Returns the number of vertices forming the face.
    */
    int numVertices() const;        

    /*!
    Returns the vertex index at the i-th position. First vertex has index 0. 
    */
    int vertexIndex(int i) const;   

    /*! 
    Returns the current normal vector of the face (undefined by default). 
    The return value is thus undefined unless computeNormal() has been called for this face.
    */
    Vector normal() const;          

    /*! 
    Recomputes the normal vector of the face. This method must be called explicitly
    after constructing/modifying the face.
    \param verts contains actual vertex coordinates from the object containing the face.
    */
    void computeNormal(const vector<Vertex> &verts);

private:
    // atributs
    Vector pnormal;         // normal de la cara    
    vector<int> pvertices;  // vector amb els indexs dels vertexs de la cara
};

#endif
