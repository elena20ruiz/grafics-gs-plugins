#ifndef _DRAW_VBO_EXT_H
#define _DRAW_VBO_EXT_H

#include <vector>
#include "basicplugin.h"

using namespace std;

class DrawVBO : public QObject, public BasicPlugin
{
     Q_OBJECT
#if QT_VERSION >= 0x050000
     Q_PLUGIN_METADATA(IID "BasicPlugin")   
#endif
     Q_INTERFACES(BasicPlugin)

 public:
    ~DrawVBO();
 
    void onPluginLoad();
    void onObjectAdd();
    void onSceneClear();
    bool drawObject(int i);
    bool drawScene();
   
 private:
    void cleanUp();
    void addVBO(unsigned int currentObject);
    void buildVF(const Object& obj, vector<vector<int> >& VF);

    // We will create a VBO for each object in the scene
    vector<GLuint> VAOs;          // ID of VAOs
    vector<GLuint> coordBuffers;  // ID of vertex coordinates buffer 
    vector<GLuint> normalBuffers; // ID of normal components buffer 
    vector<GLuint> stBuffers;     // ID of (s,t) buffer 
    vector<GLuint> colorBuffers;  // ID of color buffer  
    vector<GLuint> indexBuffers;  // ID of index buffer
    vector<GLuint> numIndices;    // Size (number of indices) in each index buffer

 };
 
 #endif
 
 
