#ifndef _TEMPLATE_H
#define _TEMPLATE_H

#include "basicplugin.h"
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QElapsedTimer>

// Documentation: http://www.cs.upc.edu/~virtual/Gdocs/interfaces/html/class_basic_plugin.html

class Template : public QObject, public BasicPlugin
 {
     Q_OBJECT
#if QT_VERSION >= 0x050000
     Q_PLUGIN_METADATA(IID "BasicPlugin")   
#endif
     Q_INTERFACES(BasicPlugin)

 public:

    void onPluginLoad();
    void onObjectAdd();
    void preFrame();
    void postFrame();
    bool paintGL();
    bool drawScene();
	
    void keyPressEvent (QKeyEvent *);
    void keyReleaseEvent (QKeyEvent *);
    void mouseMoveEvent (QMouseEvent *);
    void mousePressEvent (QMouseEvent *);
    void mouseReleaseEvent (QMouseEvent *);
    void wheelEvent (QWheelEvent *);

 public slots:

 private:
    QOpenGLShaderProgram* program;
    QOpenGLShader* vs;
    QOpenGLShader* fs;

    QTimer timer;
    QElapsedTimer elapsedTimer;
    // Add here additional members
    

 };
 
 #endif
