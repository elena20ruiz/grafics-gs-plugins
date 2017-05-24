#include "draw-bounding-box.h"
#include "glwidget.h"

void DrawBoundingBox::onPluginLoad() {
  // Carregar shader, compile & link 
  vs=new QGLShader(QGLShader::Vertex, this);
  vs->compileSourceFile("/home/llop/Llop/FIB/2015-2016QT/G/Lab/NewViewer/plugins/draw-bounding-box/draw-bounding-box.vert");
  fs=new QGLShader(QGLShader::Fragment, this);
  fs->compileSourceFile("/home/llop/Llop/FIB/2015-2016QT/G/Lab/NewViewer/plugins/draw-bounding-box/draw-bounding-box.frag");
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
void DrawBoundingBox::generate_vbo(GLuint &vbo_id, GLsizeiptr size, const GLfloat *data, GLuint attribute_id, GLint attribute_size) {
  glGenBuffers(1, &vbo_id);
  glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*size, data, GL_STATIC_DRAW);
  glVertexAttribPointer(attribute_id, attribute_size, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(attribute_id);
}

void DrawBoundingBox::createBuffers() {
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

void DrawBoundingBox::drawBoxes() {
  // crear buffers
  if (!created) {
    created=true;
    createBuffers();
  }
  
  // establir modelViewProjection
  QMatrix4x4 MVP=camera()->projectionMatrix()*camera()->modelviewMatrix();
  program->setUniformValue("modelViewProjectionMatrix", MVP);
  
  Scene* scn=scene();
  const vector<Object> &obj=scn->objects();
  for (int i=0; i<(int)obj.size(); ++i) {
    // pintar la capsa contenidora de tots els objectes de l'escena
    program->setUniformValue("boundingBoxMin",obj[i].boundingBox().min());
    program->setUniformValue("boundingBoxMax",obj[i].boundingBox().max());
    glBindVertexArray(cubeVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 18);
    glBindVertexArray(0);
  }

}

void DrawBoundingBox::postFrame() { 
  program->bind();
  GLint polygonMode;                              // en que polygon mode estamos? (GL_LINE, GL_FILL, GL_POINTS)
  glGetIntegerv(GL_POLYGON_MODE, &polygonMode);
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);      // pinta el wireframe
  drawBoxes();                                    // de las cajas contenedoras de todos los objetos de la escena
  glPolygonMode(GL_FRONT_AND_BACK, polygonMode);  // reestablecer el poygon mode anterior
  program->release();
}


#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(draw-bounding-box, DrawBoundingBox)   // plugin name, plugin class
#endif

