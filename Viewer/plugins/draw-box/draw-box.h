#ifndef _TEMPLATE_H
#define _TEMPLATE_H

#include "basicplugin.h"
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QElapsedTimer>

// Documentation: http://www.cs.upc.edu/~virtual/Gdocs/interfaces/html/class_basic_plugin.html

class DrawBox : public QObject, public BasicPlugin
 {
     Q_OBJECT
#if QT_VERSION >= 0x050000
     Q_PLUGIN_METADATA(IID "BasicPlugin")   
#endif
     Q_INTERFACES(BasicPlugin)

 public:

    void onPluginLoad();
    void postFrame();

    //PROPIS
    void drawObjecte(int i);
    bool drawScene();
    void drawBoxes();
    void cleanUp();
    void generarEscena();

 public slots:

 private:
    QOpenGLShaderProgram* program;
    QOpenGLShader* vs;
    QOpenGLShader* fs;

    QTimer timer;
    QElapsedTimer elapsedTimer;

    // Add here additional members
    vector<GLuint> VAOs;          // ID of VAOs
    vector<GLuint> coordBuffers;  // ID of vertex coordinates buffer
    vector<GLuint> colorBuffers;  // ID of color buffer
    vector<GLuint> numIndices;    // Size (number of indices) in each index buffer


 };
 
 #endif
