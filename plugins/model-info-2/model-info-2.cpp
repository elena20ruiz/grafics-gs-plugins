#include "model-info-2.h"
#include "glwidget.h"
#include <cstdio>

// número total d’objectes carregats, 
// número total de polígons, 
// número total de vèrtexs, 
// i el percentatge de polígons que són triangles
void ModelInfo2::updateModelInfo() {
  // do count
  Scene* scn=scene();
  nobj=scn->objects().size();
  npol=nver=ntri=0;
  for (int i=0; i<(int)scn->objects().size(); ++i) {
    const Object &obj=scn->objects()[i];
    for (int j=0; j<(int)obj.faces().size(); ++j) {
      const Face &face=obj.faces()[j];
      int fver=face.numVertices();
      ++npol;
      nver+=fver;
      ntri+=fver==3;
    }
  }
}

void ModelInfo2::onPluginLoad() {
  nobj_str="Objectes: \t%1";
  npol_str="Poligons: \t\t%1";
  nver_str="Verices: \t\t%1";
  perc_str="Triangles: \t%1%";
  updateModelInfo();
  // Carregar shader, compile & link 
  vs = new QGLShader(QGLShader::Vertex, this);
  vs->compileSourceFile("/home/llop/Llop/FIB/2015-2016QT/G/Lab/NewViewer/plugins/model-info-2/model-info-2.vert");
  fs = new QGLShader(QGLShader::Fragment, this);
  fs->compileSourceFile("/home/llop/Llop/FIB/2015-2016QT/G/Lab/NewViewer/plugins/model-info-2/model-info-2.frag");
  program = new QGLShaderProgram(this);
  program->addShader(vs);
  program->addShader(fs);
  program->link();
}

void ModelInfo2::onObjectAdd() {
  updateModelInfo();
}

void ModelInfo2::drawRect() {
  static bool created=false;
  static GLuint VAO_rect;
  // 1. Create VBO Buffers
  if (!created) {
    created=true;
    // Create & bind empty VAO
    glGenVertexArrays(1, &VAO_rect);
    glBindVertexArray(VAO_rect);
    float z = -0.99999;
    // Create VBO with (x,y,z) coordinates
    float coords[]={ -1, -1, z, 
                      1, -1, z, 
                     -1,  1, z, 
                      1,  1, z };
    GLuint VBO_coords;
    glGenBuffers(1, &VBO_coords);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_coords);
    glBufferData(GL_ARRAY_BUFFER, sizeof(coords), coords, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
  }
  // 2. Draw
  glBindVertexArray (VAO_rect);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
  glBindVertexArray(0);
}

void ModelInfo2::postFrame() { 
  const int SIZE=1024;
  // 1. Create image with text
  QImage image(SIZE,SIZE,QImage::Format_RGB32);
  image.fill(Qt::white);    
  QPainter painter(&image);
  QFont font;
  font.setPixelSize(32);
  painter.setFont(font);
  painter.setPen(QColor(50,50,50));
  int x=15;
	int y=50;
  painter.drawText(x, y, nobj_str.arg(nobj));
  painter.drawText(x, y+40, npol_str.arg(npol));
  painter.drawText(x, y+80, nver_str.arg(nver));
  painter.drawText(x, y+120, perc_str.arg(((double)ntri/npol)*100));    
  painter.end();
  // 2. Create texture
  const int textureUnit=5;
  glActiveTexture(GL_TEXTURE0+textureUnit);
	QImage im0=QGLWidget::convertToGLFormat(image);
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, im0.width(), im0.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, im0.bits());
  glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Pass 2. Draw quad using texture
  program->bind();
  program->setUniformValue("colorMap", textureUnit);
  program->setUniformValue("WIDTH", float(glwidget()->width()));
  program->setUniformValue("HEIGHT", float(glwidget()->height()));    
  // quad covering viewport 
  drawRect();
  program->release();
  glBindTexture(GL_TEXTURE_2D, 0);
  glDeleteTextures(1, &textureID);
}


#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(model-info-2, ModelInfo2)   // plugin name, plugin class
#endif

