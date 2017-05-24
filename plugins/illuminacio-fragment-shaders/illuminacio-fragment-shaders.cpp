#include "illuminacio-fragment-shaders.h"
#include "glwidget.h"

void IlluminacioFragmentShaders::onPluginLoad() {
  // Carregar shader, compile & link 
  vs = new QGLShader(QGLShader::Vertex, this);
  vs->compileSourceFile("/home/llop/Llop/FIB/2015-2016QT/G/Lab/NewViewer/plugins/illuminacio-fragment-shaders/lighting3.vert");
  fs = new QGLShader(QGLShader::Fragment, this);
  fs->compileSourceFile("/home/llop/Llop/FIB/2015-2016QT/G/Lab/NewViewer/plugins/illuminacio-fragment-shaders/lighting3.frag");
  program = new QGLShaderProgram(this);
  program->addShader(vs);
  program->addShader(fs);
  program->link();
}

void IlluminacioFragmentShaders::preFrame() {
  if (program && program->isLinked()) {
    program->bind();
    const Camera* cam=camera();
    program->setUniformValue("modelViewProjectionMatrix", cam->projectionMatrix()*cam->modelviewMatrix());
    program->setUniformValue("modelViewMatrix", cam->modelviewMatrix());
    program->setUniformValue("normalMatrix", cam->modelviewMatrix().normalMatrix());
    //lightAmbient = Vector(0.1,0.1,0.1);
    //lightDiffuse = Vector(1,1,1);
    //lightSpecular = Vector(1,1,1);
    //lightPosition = QVector4D(0,0,0,1);
    program->setUniformValue("lightAmbient", QVector4D(Vector(0.1,0.1,0.1),1));
    program->setUniformValue("lightDiffuse", QVector4D(Vector(1,1,1),1));
    program->setUniformValue("lightSpecular", QVector4D(Vector(1,1,1),1));
    program->setUniformValue("lightPosition", QVector4D(0,0,0,1));
    //materialAmbient = Vector(0.8, 0.8, 0.6);
    //materialDiffuse = Vector(0.8, 0.8, 0.6);
    //materialSpecular = Vector(1.0, 1.0, 1.0);
    //materialShininess = 64.0;
    program->setUniformValue("matAmbient", QVector4D(Vector(0.8, 0.8, 0.6),1));
    program->setUniformValue("matDiffuse", QVector4D(Vector(0.8, 0.8, 0.6),1));
    program->setUniformValue("matSpecular", QVector4D(Vector(1.0, 1.0, 1.0),1));
    program->setUniformValue("matShininess", GLfloat(64.0));
  }
}

void IlluminacioFragmentShaders::postFrame() { 
  if (program && program->isLinked()) program->release();
}


#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(illuminacio-fragment-shaders, IlluminacioFragmentShaders)   // plugin name, plugin class
#endif

