#ifndef _TEMPLATE_H
#define _TEMPLATE_H

#include "basicplugin.h"
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QElapsedTimer>

// Documentation: http://www.cs.upc.edu/~virtual/Gdocs/interfaces/html/class_basic_plugin.html

class Light : public QObject, public BasicPlugin {
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

    Vector materialAmbient;
    Vector materialDiffuse;
    Vector materialSpecular;
    float materialShininess;

    Vector lightAmbient;
    Vector lightDiffuse;
    Vector lightSpecular;
    QVector4D lightPosition;   // eye space or world space, depending on boolean
    bool lightEncodedInEyeSpace;

};
 
 #endif
