#ifndef _TEMPLATE_H
#define _TEMPLATE_H

#include "basicplugin.h"

class KeyboardSelection : public QObject, public BasicPlugin {
    Q_OBJECT
#if QT_VERSION >= 0x050000
    Q_PLUGIN_METADATA(IID "BasicPlugin")
#endif
    Q_INTERFACES(BasicPlugin)

public:
    void keyPressEvent (QKeyEvent *);
public slots:

private:

};

#endif
