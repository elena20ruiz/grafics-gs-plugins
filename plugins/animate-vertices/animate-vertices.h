#ifndef _ANIMATEVERTICES_H
#define _ANIMATEVERTICES_H

#include "basicplugin.h"
#include <QGLShader>
#include <QGLShaderProgram>
#include <QElapsedTimer>

class AnimateVertices : public QObject, public BasicPlugin {
     Q_OBJECT
     Q_INTERFACES(BasicPlugin)

 public:
    void onPluginLoad();
    void preFrame();
    void postFrame();
    
 private:
    QGLShaderProgram* program;
    QGLShader *fs, *vs; 
    QElapsedTimer elapsedTimer;
 };
 
 #endif
