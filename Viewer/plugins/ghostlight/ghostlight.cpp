#include "ghostlight.h"
#include "glwidget.h"

void ghostlight::preFrame()
{
    glDisable(GL_DEPTH_TEST);
    glBlendEquation(GL_FUNC_ADD);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
}

void ghostlight::postFrame()
{
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
}


void SelectionMouse::mouseReleaseEvent (QMouseEvent *event) {

    // Comprovar que efectivament s’ha fet click amb el botó adient i els modificadors (Shift,
   // Control...) adients.

      if ((event->button() & Qt::LeftButton)) {
          GLWidget &g=*glwidget();
          g.makeCurrent();

          //PER ESCBORRAR!!!----------------------------------
          g.glClearColor(0, 0, 0, 0);
          g.glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
         //---------------------------------------------------
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

      }

    g.update();
}




void ghostlight::onPluginLoad()
{
    glwidget()->makeCurrent();
    // Carregar shader, compile & link
    vs = new QOpenGLShader(QOpenGLShader::Vertex, this);
    vs->compileSourceFile("plugins/ghostlight/ghostlight.vert");

    fs = new QOpenGLShader(QOpenGLShader::Fragment, this);
    fs->compileSourceFile("plugins/ghostlight/ghostlight.frag");

    program = new QOpenGLShaderProgram(this);
    program->addShader(vs);
    program->addShader(fs);
    program->link();
}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(alpha-blending, AlphaBlending)   // plugin name, plugin class
#endif

