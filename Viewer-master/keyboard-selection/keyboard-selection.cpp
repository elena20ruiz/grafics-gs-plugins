#include "keyboard-selection.h"

void KeyboardSelection::keyPressEvent (QKeyEvent *e) {
    int numPressed;
    switch (e->key()) {
    case Qt::Key_0:
        numPressed = 0;
        break;
    case Qt::Key_1:
        numPressed = 1;
        break;
    case Qt::Key_2:
        numPressed = 2;
        break;
    case Qt::Key_3:
        numPressed = 3;
        break;
    case Qt::Key_4:
        numPressed = 4;
        break;
    case Qt::Key_5:
        numPressed = 5;
        break;
    case Qt::Key_6:
        numPressed = 6;
        break;
    case Qt::Key_7:
        numPressed = 7;
        break;
    case Qt::Key_8:
        numPressed = 8;
        break;
    case Qt::Key_9:
        numPressed = 9;
        break;
    default: return;
    }
    if (numPressed < scene()->objects().size()) {
        scene()->setSelectedObject(numPressed);
    }
    else scene()->setSelectedObject(-1);
    glwidget()->update();
}

#if QT_VERSION < 0x040000
Q_EXPORT_PLUGIN2(keyboard-selection, KeyboardSelection)   // plugin name, plugin class
#endif
