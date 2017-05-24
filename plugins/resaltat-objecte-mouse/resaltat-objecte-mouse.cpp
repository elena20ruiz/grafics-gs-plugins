#include "resaltat-objecte-mouse.h"
#include "glwidget.h"

void ResaltatObjecteMouse::setSelectedObject(int selected) {
  if (selected<0 || selected>=(int)scene()->objects().size()) selected=-1;
  scene()->setSelectedObject(selected);
}

void encodeID(int index, GLubyte *color) {
  color[0]=color[1]=color[2]=index;
}

// maxim 255 objectes. com que el valor de cada color va entre 0 i 255:
// 0-254 id objecte
// 255 cap objecte (el color de fons es blanc)
int decodeID(GLubyte *color) {
  if (color[0]==255) return -1;
  return color[0];
}

void ResaltatObjecteMouse::mouseReleaseEvent(QMouseEvent* e) {
  if (!(e->button()&Qt::LeftButton)) return;          // boto esquerre del ratoli
  if (e->modifiers()&(Qt::ShiftModifier)) return;     // res de tenir shift pulsat
  if (!(e->modifiers()&Qt::ControlModifier)) return;  // la tecla control ha d'estar pulsada
  
  glClearColor(1, 1, 1, 1);                           // esborrar els buffers amb un color de fons únic (blanc)
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
  
  programColor->bind();                               // activar (bind) el shader program amb el VS+FS d’abans
  drawDrawColorScene();
  programColor->release();

  int x=e->x();                                       // llegir el color del buffer de color sota la posició del cursor
  int y=glwidget()->height()-e->y();
  GLubyte read[4];
  glReadPixels(x, y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, read);
  
  int seleccio=decodeID(&read[0]);                    // obtenir l'identificador de l'objecte corresponent i, 
  setSelectedObject(seleccio);                        // si no és color de fons, establir l'objecte seleccionat
  
  glwidget()->updateGL();                             // cridar a updateGL per tal que es repinti l'escena
}

void ResaltatObjecteMouse::onPluginLoad() {
  // Carregar shader, compile & link (capsa contenidora)
  vsBox=new QGLShader(QGLShader::Vertex, this);
  vsBox->compileSourceFile("/home/llop/Llop/FIB/2015-2016QT/G/Lab/NewViewer/plugins/resaltat-objecte-mouse/resaltat-objecte-mouse-box.vert");
  fsBox=new QGLShader(QGLShader::Fragment, this);
  fsBox->compileSourceFile("/home/llop/Llop/FIB/2015-2016QT/G/Lab/NewViewer/plugins/resaltat-objecte-mouse/resaltat-objecte-mouse-box.frag");
  programBox=new QGLShaderProgram(this);
  programBox->addShader(vsBox);
  programBox->addShader(fsBox);
  programBox->link();
  // Carregar shader, compile & link (escena colors unics)
  vsColor=new QGLShader(QGLShader::Vertex, this);
  vsColor->compileSourceFile("/home/llop/Llop/FIB/2015-2016QT/G/Lab/NewViewer/plugins/resaltat-objecte-mouse/resaltat-objecte-mouse.vert");
  fsColor=new QGLShader(QGLShader::Fragment, this);
  fsColor->compileSourceFile("/home/llop/Llop/FIB/2015-2016QT/G/Lab/NewViewer/plugins/resaltat-objecte-mouse/resaltat-objecte-mouse.frag");
  programColor=new QGLShaderProgram(this);
  programColor->addShader(vsColor);
  programColor->addShader(fsColor);
  programColor->link();
  created=false;
}

// Pintar l'escena assegurant-se que cada objecte es pinta amb un color únic 
// que permeti identificar l'objecte (i diferent del color de fons)
void ResaltatObjecteMouse::drawDrawColorScene() {
  // enviar la modelViewProjectionMatrix, i el color identificador dels objectes
  QMatrix4x4 MVP=camera()->projectionMatrix()*camera()->modelviewMatrix();
  programColor->setUniformValue("modelViewProjectionMatrix", MVP);
  for (unsigned int i=0; i<scene()->objects().size(); ++i) {
    GLubyte color[4];
    encodeID(i, color);
    programColor->setUniformValue("color", QVector4D(color[0]/255., color[1]/255., color[2]/255., 1));
    drawPlugin()->drawObject(i);
  }
}

// rellena un vbo con los datos proporcionados:
// vbo_id = identificador del vbo
// size = número de elementos del array de datos
// data = array de datos
// attribute_id = identificador del atributo correspondiente del shader
// attribute_size = número de elementos que componen el atributo (ie. 3 para vértices)
// Se asume que los datos del array serán de tipo float
void ResaltatObjecteMouse::generate_vbo(GLuint &vbo_id, GLsizeiptr size, const GLfloat *data, GLuint attribute_id, GLint attribute_size) {
  glGenBuffers(1, &vbo_id);
  glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*size, data, GL_STATIC_DRAW);
  glVertexAttribPointer(attribute_id, attribute_size, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(attribute_id);
}

void ResaltatObjecteMouse::createBuffers() {
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

void ResaltatObjecteMouse::drawSelectedBox() {
  int seleccionat=scene()->selectedObject();
  if (seleccionat>=0 && seleccionat<(int)scene()->objects().size()) {
    // crear buffers
    if (!created) {
      created=true;
      createBuffers();
    }
    
    // establir modelViewProjection
    QMatrix4x4 MVP=camera()->projectionMatrix()*camera()->modelviewMatrix();
    programBox->setUniformValue("modelViewProjectionMatrix", MVP);
    
    const Object &obj=scene()->objects()[seleccionat];
    // pintar la capsa contenidora de tots els objectes de l'escena
    programBox->setUniformValue("boundingBoxMin",obj.boundingBox().min());
    programBox->setUniformValue("boundingBoxMax",obj.boundingBox().max());
    glBindVertexArray(cubeVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 18);
    glBindVertexArray(0);
  }
}

void ResaltatObjecteMouse::postFrame() {
  programBox->bind();
  GLint polygonMode;                              // en que polygon mode estamos? (GL_LINE, GL_FILL, GL_POINTS)
  glGetIntegerv(GL_POLYGON_MODE, &polygonMode);
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);      // pinta el wireframe
  drawSelectedBox();                              // de la caja contenedora del objeto seleccionado de la escena
  glPolygonMode(GL_FRONT_AND_BACK, polygonMode);  // reestablecer el poygon mode anterior
  programBox->release();
}


#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(resaltat-objecte-mouse, ResaltatObjecteMouse)   // plugin name, plugin class
#endif
