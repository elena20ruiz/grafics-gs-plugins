#include "mouse-selection.h"

void MouseSelection::onPluginLoad() {
    // Carregar shader, compile & link
    QOpenGLShader* vs = new QOpenGLShader(QOpenGLShader::Vertex, this);
    vs->compileSourceFile("plugins/mouse-selection/mouse-selection.vert");
    cout << "VS log:" << vs->log().toStdString() << endl;

    QOpenGLShader* fs = new QOpenGLShader(QOpenGLShader::Fragment, this);
    fs->compileSourceFile("plugins/mouse-selection/mouse-selection.frag");
    cout << "FS log:" << fs->log().toStdString() << endl;

    program = new QOpenGLShaderProgram(this);
    program->addShader(vs);
    program->addShader(fs);
    program->link();
    cout << "Link log:" << program->log().toStdString() << endl;
}

void MouseSelection::mouseReleaseEvent(QMouseEvent* event) {
    if (!(event->button() & Qt::RightButton)) return;
    if (event->modifiers() & (Qt::ShiftModifier)) return;
    if (!(event->modifiers() & Qt::ControlModifier)) return;
    GLWidget &g = *glwidget();
    g.makeCurrent();

    g.glClearColor(0, 0, 0, 0);
    g.glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    program->bind();
    program->setUniformValue("modelViewProjectionMatrix", camera()->projectionMatrix() * camera()->viewMatrix());

    for (unsigned int i=0; i<scene()->objects().size(); ++i) {
        GLubyte color[4];
        color[0] = i+1;
        color[1] = 0;
        color[2] = 0;
        program->setUniformValue("color", QVector4D(color[0]/255.,
                                 color[1], color[2], 1.0));
        drawPlugin()->drawObject(i);
    }
    program->release();
    int x = event->x();
    int y = glwidget()->height() - event->y();
    GLubyte read[4];
    g.glReadPixels(x, y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, read);

    unsigned int selected = read[0];

    if (selected > 0) scene()->setSelectedObject(selected-1);
    else scene()->setSelectedObject(-1);

    g.update();
}

#if QT_VERSION < 0x040000
Q_EXPORT_PLUGIN2(mouse-selection, MouseSelection)   // plugin name, plugin class
#endif
