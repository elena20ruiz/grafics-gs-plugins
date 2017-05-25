#ifndef _TEMPLATE_H
#define _TEMPLATE_H

#include "basicplugin.h"

class MouseSelection : public QObject, public BasicPlugin {
    Q_OBJECT
#if QT_VERSION >= 0x050000
    Q_PLUGIN_METADATA(IID "BasicPlugin")
#endif
    Q_INTERFACES(BasicPlugin)

public:
    void onPluginLoad();
    void mouseReleaseEvent (QMouseEvent*);
public slots:

private:
    QOpenGLShaderProgram* program;
};

#endif
