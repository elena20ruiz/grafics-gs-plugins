#include "draw-smooth.h"
#include "glwidget.h"
#include <cmath>

void DrawSmooth::cleanUp() {
	glDeleteBuffers(coordBuffers.size(),  &coordBuffers[0]);
	glDeleteBuffers(normalBuffers.size(), &normalBuffers[0]);
  glDeleteBuffers(indexBuffers.size(),  &indexBuffers[0]);
  glDeleteBuffers(stBuffers.size(),  &stBuffers[0]);
  glDeleteBuffers(colorBuffers.size(),  &colorBuffers[0]);

  glDeleteVertexArrays(VAOs.size(), &VAOs[0]);
  
  coordBuffers.clear();
  normalBuffers.clear();
  indexBuffers.clear();
  stBuffers.clear();
  colorBuffers.clear();
  VAOs.clear();

  numIndices.clear(); 
}

DrawSmooth::~DrawSmooth() {
  cleanUp();
}

void DrawSmooth::onSceneClear() {
  cleanUp();
}

bool DrawSmooth::drawObject(int i) {
  glBindVertexArray(VAOs[i]);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffers[i]);
	glDrawElements(GL_TRIANGLES, numIndices[i], GL_UNSIGNED_INT, (GLvoid*) 0);

  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER,0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
  return true;
}

bool DrawSmooth::drawScene() {
	for (unsigned int i=0; i<coordBuffers.size(); ++i)  {   // for each buffer (that is, for each object)
    glBindVertexArray(VAOs[i]);
		glDrawElements(GL_TRIANGLES, numIndices[i], GL_UNSIGNED_INT, (GLvoid*) 0);
	}
  glBindVertexArray(0);
  return true;
}

void DrawSmooth::onPluginLoad() {
  for(unsigned int i=0; i<scene()->objects().size(); ++i) addVBO(i);
}

void DrawSmooth::onObjectAdd() {
  addVBO(scene()->objects().size()-1);
}

void DrawSmooth::addVBO(unsigned int currentObject) {  
  // Step 1: Create and fill the four arrays (vertex coords, vertex normals, and face indices) 
  // This version: 
  //  - each coord/normal will appear n times (one for each corner with unique normal)
  //  - non-triangular faces (convexity is assumed) are triangulated on-the-fly: (v0,v1,v2,v3) -> (v0,v1,v2) (v0,v2,v3)
  vector<float> vertices; // (x,y,z)    Final size: 3*number of corners
  vector<float> normals;  // (nx,ny,nz) Final size: 3*number of corners
  vector<float> st;       // (s,t)      Final size: 2*number of corners  
  vector<float> colors;   // (r,g,b)    Final size: 3*number of corners
	vector<int> indices;    //            Final size: 3*number of triangles  
  
  // V'=V. Això implica que cada vèrtex (x,y,z) apareix un sol cop al VBO (sense repetits), amb una única normal.
  // Per tant, totes les cares que fan servir aquest vèrtex el rebran amb la mateixa normal.
  // El mateix s’aplica a la resta d’atributs (color i coordenades de textura)
  const Object &obj=scene()->objects()[currentObject];
  const vector<Vertex> &verts=obj.vertices();
  const vector<Face> &faces=obj.faces();
  // fem que la normal d'un vertex sigui la mitja de les normals de les cares on surt aquest vertex
  vector<Vector> VN(obj.vertices().size(), Vector(0,0,0));    // suma de les normals de les diferents cares on apareixen els vertex
  vector<int> VC(obj.vertices().size(), 0);                   // conte de les normals de les diferents cares on apareixen els vertex
  for (int i=0; i<(int)faces.size(); ++i) {
    const Face &face=faces[i];
    const Vector &N=face.normal().normalized();
    for (int j=0; j<(int)face.numVertices(); ++j) {
      VN[face.vertexIndex(j)]+=N; 
      ++VC[face.vertexIndex(j)];
    }
  }
  // els vertex i demes nomes apareixen 1 cop al VBO
  for (int i=0; i<(int)verts.size(); ++i) {
    Vector N=VN[i]/VC[i];
    const Point &P=verts[i].coord();
    vertices.push_back(P.x()); vertices.push_back(P.y()); vertices.push_back(P.z());
    normals.push_back(N.x()); normals.push_back(N.y()); normals.push_back(N.z());
    float r=obj.boundingBox().radius();
    float s=Vector::dotProduct((1.0/r)*Vector(1,0,1), P);
    float t=Vector::dotProduct((1.0/r)*Vector(0,1,0), P);
    if ((int)verts.size()==81) {    // plane: special case for /assig models
      s=0.5f*(P.x()+1.0);
      t=0.5f*(P.y()+1.0);
    }
    if ((int)verts.size()==386) {   // cube: special case for /assig models
      s=Vector::dotProduct((1.0/2.0)*Vector(1,0,1), P);
      t=Vector::dotProduct((1.0/2.0)*Vector(0,1,0), P);
    }
    st.push_back(s);
    st.push_back(t);
    colors.push_back(abs(N.x()));
    colors.push_back(abs(N.y()));
    colors.push_back(abs(N.z()));
  }
  // els index els hem de posar en ordre, pero
  for (int i=0; i<(int)faces.size(); ++i) {
    const Face &face=faces[i];
    for (int j=0; j<(int)face.numVertices(); ++j) 
      indices.push_back(face.vertexIndex(j));
  }

  // Step 2: Create empty buffers (coords, normals, st, indices)
  GLuint VAO;
  glGenVertexArrays(1, &VAO);
  VAOs.push_back(VAO);
  glBindVertexArray(VAO);

  GLuint coordBufferID;
	glGenBuffers(1, &coordBufferID);
  coordBuffers.push_back(coordBufferID);

  GLuint normalBufferID;
	glGenBuffers(1, &normalBufferID);
  normalBuffers.push_back(normalBufferID);

  GLuint colorBufferID;
	glGenBuffers(1, &colorBufferID);
  colorBuffers.push_back(colorBufferID);

  GLuint stBufferID;
	glGenBuffers(1, &stBufferID);
  stBuffers.push_back(stBufferID);

  GLuint indexBufferID;
	glGenBuffers(1, &indexBufferID);
  indexBuffers.push_back(indexBufferID);

	numIndices.push_back(indices.size());

  // Step 3: Define VBO data (coords, normals, indices)
	glBindBuffer(GL_ARRAY_BUFFER, coordBuffers[currentObject]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*vertices.size(), &vertices[0], GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);  // VAO
  glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, normalBuffers[currentObject]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*normals.size(), &normals[0], GL_STATIC_DRAW);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);  // VAO
  glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, colorBuffers[currentObject]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*colors.size(), &colors[0], GL_STATIC_DRAW);
  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);  // VAO
  glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, stBuffers[currentObject]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*st.size(), &st[0], GL_STATIC_DRAW);
  glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 0, 0);  // VAO
  glEnableVertexAttribArray(3);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffers[currentObject]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int)*indices.size(), &indices[0], GL_STATIC_DRAW);

  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER,0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);

}

#if QT_VERSION < 0x050000
    Q_EXPORT_PLUGIN2(draw-ssmooth, DrawSmooth)   // plugin name, plugin class
#endif



