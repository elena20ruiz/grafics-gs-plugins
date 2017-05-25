#ifndef _TEMPLATE_H
#define _TEMPLATE_H

#include "basicplugin.h"


// Documentation: http://www.cs.upc.edu/~virtual/Gdocs/interfaces/html/class_basic_plugin.html

class SelectionMouse : public QObject, public BasicPlugin
 {
     Q_OBJECT
#if QT_VERSION >= 0x050000
     Q_PLUGIN_METADATA(IID "BasicPlugin")   
#endif
     Q_INTERFACES(BasicPlugin)

 public:
    void mouseReleaseEvent(QMouseEvent *);
    void onPluginLoad();

 public slots:

 private:
    QOpenGLShaderProgram* program;

    QOpenGLShader* vs;
    QOpenGLShader* fs;

 };
 
 #endif
