#include "framerate.h"
#include "glwidget.h"

// funcion declarada como slot en el .h
void Framerate::updateFPS() {
  fps=fcnt;                 // establecer FPS
  fcnt=0;                   // reset contador
  glwidget()->updateGL();   // refrescar pantalla
}

void Framerate::onPluginLoad() {
  // Carregar shader, compile & link 
  vs=new QGLShader(QGLShader::Vertex, this);
  vs->compileSourceFile("/home/llop/Llop/FIB/2015-2016QT/G/Lab/NewViewer/plugins/framerate/framerate.vert");
  fs=new QGLShader(QGLShader::Fragment, this);
  fs->compileSourceFile("/home/llop/Llop/FIB/2015-2016QT/G/Lab/NewViewer/plugins/framerate/framerate.frag");
  program=new QGLShaderProgram(this);
  program->addShader(vs);
  program->addShader(fs);
  program->link();
  // update frame count every second
  fps=fcnt=0;
  QTimer *frameTimer=new QTimer(this);
  connect(frameTimer, SIGNAL(timeout()), this, SLOT(updateFPS()));
  frameTimer->start(1000);
  // keep repainting
  QTimer *paintTimer=new QTimer(this);
  connect(paintTimer, SIGNAL(timeout()), glwidget(), SLOT(updateGL()));
  paintTimer->start();
}

void Framerate::drawRect() {
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

void Framerate::postFrame() { 
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
  painter.drawText(x, y, QString("FPS: %1").arg(fps));
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
  // increment frame counter
  ++fcnt;
}


#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(framerate, Framerate)   // plugin name, plugin class
#endif

