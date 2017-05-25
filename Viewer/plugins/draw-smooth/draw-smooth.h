#ifndef _DRAW_VBO_EXT_H
#define _DRAW_VBO_EXT_H

#include <vector>
#include "basicplugin.h"

using namespace std;

class DrawSmooth : public QObject, public BasicPlugin
{
     Q_OBJECT
#if QT_VERSION >= 0x050000
     Q_PLUGIN_METADATA(IID "BasicPlugin")   
#endif
     Q_INTERFACES(BasicPlugin)

 public:
    ~DrawSmooth();
 
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
    vector<GLuint> VAOs;
    vector<unsigned int> numIndices;
    vector<GLuint> vertexBuffer;
    vector<GLuint> normalBuffer;
    vector<GLuint> colorBuffer;
    vector<GLuint> indexBuffer;
 };
 
 #endif
 
 
