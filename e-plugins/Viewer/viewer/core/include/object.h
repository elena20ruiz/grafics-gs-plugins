#ifndef OBJECT_H
#define OBJECT_H

#include <vector>
#include "box.h"
#include "face.h"
//#include <QtOpenGL/qgl.h>

using namespace std;

/*!
	\class Object
	\brief The %Object class represents a 3D object. 

    Each object consists of a collection of vertices and a collection of faces. 
    Objects also store a bounding box. 
*/

class CORE_EXPORT Object
{
public:
    /*!
    Constructs an empty object.
    \param name is the name of the new object. 
    */
    Object(std::string name);

    /*!
    Imports a Wavefront OBJ file as a single object. 
    Previous faces and vertices, if any, are deleted.
    */
    void readObj(const char* filename); 

    /*!
    Returns the value of the bounding box property. 
    This property is undefined unless computeBoundingBox() has been called.
    */
    Box boundingBox() const;  
    
    /*!
    Returns a constant reference (read-only) to the vector of faces of the object. 
    */
    const vector<Face>& faces() const;  

    /*!
    Returns a constant reference (read-only) to the vector of vertices of the object. 
    */
    const vector<Vertex>& vertices() const;

    /*!
    Returns a reference to the vector of vertices of the object. 
    If vertices are modified, computeNormals() and computeBoundingBox() methods should 
    be called to update per-face normals and the object's bounding box. 
    */
    vector<Vertex>& vertices();  
    
    /*!
    Recomputes per-face normals (for all faces of the object).
    */
    void computeNormals();     

    /*!
    Recomputes the object bounding box.
    */
    void computeBoundingBox(); 

    /*!
    Applies geometric transform to all vertices; updates normals & bounding box.
    */
    void applyGT(const QMatrix4x4& mat);

private:
    void make_face ( char **words, int nwords );
    void clear();

    vector<Vertex> pvertices;  // vector amb els vertexs de l'objecte
    vector<Face> pfaces;       // vector amb les cares de l'objecte

    std::string pname;
    Box pboundingBox;   // caixa contenidora de l'objecte.
};

#endif

