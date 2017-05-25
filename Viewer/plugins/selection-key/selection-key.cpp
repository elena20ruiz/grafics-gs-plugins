#include "selection-key.h"
#include <cstdlib>

// Documentation: http://www.cs.upc.edu/~virtual/Gdocs/interfaces/html/class_basic_plugin.html

void SelectionMouse::keyPressEvent (QKeyEvent *e) {
    GLWidget &g=*glwidget();
    g.makeCurrent();
    int numPressed;
    switch (e->key()) {
    case Qt::Key_0:
        numPressed = 0;
            cout << "0" << endl;
        break;
    case Qt::Key_1:
        numPressed = 1;
            cout << "1" << endl;
        break;
    case Qt::Key_2:
        numPressed = 2;
            cout << "2" << endl;
        break;
    case Qt::Key_3:
        numPressed = 3;
            cout << "3" << endl;
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

    Scene &s = *scene();
    if (numPressed < s.objects().size()) {
        scene()->setSelectedObject(numPressed);
    }
    else s.setSelectedObject(-1);
    g.update();
}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(selection-key, SelectionKey)   // plugin name, plugin class
#endif
