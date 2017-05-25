#ifndef _GLOWING_H
#define _GLOWING_H

#include "basicplugin.h"
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>


class Glowing : public QObject, public BasicPlugin
{
     Q_OBJECT
     Q_PLUGIN_METADATA(IID "BasicPlugin")   
     Q_INTERFACES(BasicPlugin)

 public:
    void onPluginLoad();
    bool paintGL();
 
 private:
    QOpenGLShaderProgram* program;
    QOpenGLShader* vs;
    QOpenGLShader* fs;  
    GLuint textureId;
 };
 
 #endif
