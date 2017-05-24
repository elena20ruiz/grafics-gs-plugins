#include "resaltat-objecte-teclat.h"
#include "glwidget.h"

void ResaltatObjecteTeclat::setSelectedObject(int selected) {
  if (selected<0 || selected>=(int)scene()->objects().size()) selected=-1;
  scene()->setSelectedObject(selected);
  glwidget()->updateGL();
}

void ResaltatObjecteTeclat::keyPressEvent(QKeyEvent*e) {
  if (e->key()>=Qt::Key_0 && e->key()<=Qt::Key_9) setSelectedObject(e->key()-Qt::Key_0);
}

void ResaltatObjecteTeclat::onPluginLoad() {
  // Carregar shader, compile & link 
  vs=new QGLShader(QGLShader::Vertex, this);
  vs->compileSourceFile("/home/llop/Llop/FIB/2015-2016QT/G/Lab/NewViewer/plugins/resaltat-objecte-teclat/resaltat-objecte-teclat.vert");
  fs=new QGLShader(QGLShader::Fragment, this);
  fs->compileSourceFile("/home/llop/Llop/FIB/2015-2016QT/G/Lab/NewViewer/plugins/resaltat-objecte-teclat/resaltat-objecte-teclat.frag");
  program=new QGLShaderProgram(this);
  program->addShader(vs);
  program->addShader(fs);
  program->link();
  created=false;
}

// rellena un vbo con los datos proporcionados:
// vbo_id = identificador del vbo
// size = número de elementos del array de datos
// data = array de datos
// attribute_id = identificador del atributo correspondiente del shader
// attribute_size = número de elementos que componen el atributo (ie. 3 para vértices)
// Se asume que los datos del array serán de tipo float
void ResaltatObjecteTeclat::generate_vbo(GLuint &vbo_id, GLsizeiptr size, const GLfloat *data, GLuint attribute_id, GLint attribute_size) {
  glGenBuffers(1, &vbo_id);
  glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*size, data, GL_STATIC_DRAW);
  glVertexAttribPointer(attribute_id, attribute_size, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(attribute_id);
}

void ResaltatObjecteTeclat::createBuffers() {
  // dades
  GLfloat cube_vertices[]={
    1, 1, 1,
    0, 1, 1,
    1, 0, 1,
    0, 0, 1,
    1, 0, 0,
    0, 0, 0,
    1, 1, 0,
    0, 1, 0,
    1, 1, 1,
    0, 1, 1,
    0, 1, 1,
    0, 1, 0,
    0, 0, 1,
    0, 0, 0,
    1, 0, 1,
    1, 0, 0,
    1, 1, 1,
    1, 1, 0
  };
  GLfloat cube_colors[]={
    1, 0, 0,
    1, 0, 0,
    1, 0, 0,
    1, 0, 0,
    1, 0, 0,
    1, 0, 0,
    1, 0, 0,
    1, 0, 0,
    1, 0, 0,
    1, 0, 0,
    1, 0, 0,
    1, 0, 0,
    1, 0, 0,
    1, 0, 0,
    1, 0, 0,
    1, 0, 0,
    1, 0, 0,
    1, 0, 0
  };
  // VAO Capsa
  glGenVertexArrays(1, &cubeVAO);
  glBindVertexArray(cubeVAO);
  // VBOs Capsa
  generate_vbo(verticesVBO, 18*3, &cube_vertices[0], 0, 3);
  generate_vbo(colorVBO, 18*3, &cube_colors[0], 2, 3);
}

void ResaltatObjecteTeclat::drawSelectedBox() {
  int seleccionat=scene()->selectedObject();
  if (seleccionat>=0 && seleccionat<(int)scene()->objects().size()) {
    // crear buffers
    if (!created) {
      created=true;
      createBuffers();
    }
    
    // establir modelViewProjection
    QMatrix4x4 MVP=camera()->projectionMatrix()*camera()->modelviewMatrix();
    program->setUniformValue("modelViewProjectionMatrix", MVP);
    
    const Object &obj=scene()->objects()[seleccionat];
    // pintar la capsa contenidora de tots els objectes de l'escena
    program->setUniformValue("boundingBoxMin",obj.boundingBox().min());
    program->setUniformValue("boundingBoxMax",obj.boundingBox().max());
    glBindVertexArray(cubeVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 18);
    glBindVertexArray(0);
  }
}

void ResaltatObjecteTeclat::postFrame() { 
  program->bind();
  GLint polygonMode;                              // en que polygon mode estamos? (GL_LINE, GL_FILL, GL_POINTS)
  glGetIntegerv(GL_POLYGON_MODE, &polygonMode);
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);      // pinta el wireframe
  drawSelectedBox();                              // de la caja contenedora del objeto seleccionado de la escena
  glPolygonMode(GL_FRONT_AND_BACK, polygonMode);  // reestablecer el poygon mode anterior
  program->release();
}


#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(resaltat-objecte-teclat, ResaltatObjecteTeclat)   // plugin name, plugin class
#endif

