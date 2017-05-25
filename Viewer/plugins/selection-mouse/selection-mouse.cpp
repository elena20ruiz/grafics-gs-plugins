#include "selection-mouse.h"
#include <cstdlib>

// Documentation: http://www.cs.upc.edu/~virtual/Gdocs/interfaces/html/class_basic_plugin.html


void SelectionMouse::onPluginLoad()
{

    glwidget()->makeCurrent();
    program = new QOpenGLShaderProgram(this);

    //VERTEX SHADER:
    vs = new QOpenGLShader(QOpenGLShader::Vertex, this);
    vs->compileSourceFile("plugins/selection-mouse/mouse-selection.vert");
    program->addShader(vs);

    //FRAMENT SHADER:
    fs = new QOpenGLShader(QOpenGLShader::Fragment, this);
    fs->compileSourceFile("plugins/selection-mouse/mouse-selection.frag");
    program->addShader(fs);
    program->link();
    qDebug() << program->log() << endl;
}



void SelectionMouse::mouseReleaseEvent (QMouseEvent *event) {

    // Comprovar que efectivament s’ha fet click amb el botó adient i els modificadors (Shift,
   // Control...) adients.

      if (! (event->button() & Qt::RightButton)) return;
      if ( event->modifiers() & (Qt::ShiftModifier)) return;
      if (!(event->modifiers() & Qt::ControlModifier)) return;

    GLWidget &g=*glwidget();
    g.makeCurrent();

     //PER ESCBORRAR!!!----------------------------------
     g.glClearColor(0, 0, 0, 0);
     g.glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //---------------------------------------------------

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

     program->release(); //S'ha acabat l'interacció amb uniforms

     int x = event->x();
     int y = glwidget()->height()-event->y();
     GLubyte read[4];
     glReadPixels(x, y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, read);

     unsigned int selected = read[0];

     if (selected > 0) scene()->setSelectedObject(selected-1);
     else scene()->setSelectedObject(-1);

    g.update();
}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(selection-mouse, SelectionMouse)   // plugin name, plugin class
#endif
