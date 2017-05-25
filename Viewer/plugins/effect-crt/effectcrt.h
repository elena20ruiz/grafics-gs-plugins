#ifndef _EFFECTCRT_H
#define _EFFECTCRT_H

#include "basicplugin.h"
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>


class EffectCRT : public QObject, public BasicPlugin
 {
     Q_OBJECT
#if QT_VERSION >= 0x050000
     Q_PLUGIN_METADATA(IID "BasicPlugin")   
#endif     
     Q_INTERFACES(BasicPlugin)

 public:
    void onPluginLoad();
    void preFrame();
    void postFrame();
    
 private:
    QOpenGLShaderProgram* program;
    QOpenGLShader *fs, *vs; 
 };
 
 #endif
