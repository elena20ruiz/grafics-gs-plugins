#include "multitex.h"
#include <QFileDialog>
 
void Multitex::onPluginLoad()
{
    GLWidget &g = *glwidget();
    g.makeCurrent();
	// VS
    QString vs_src = \
      "#version 330 core\n"
      "uniform float radius = 1.0;"
      "uniform mat4 modelViewProjectionMatrix;"
      "layout (location = 0) in vec3 vertex;"
      "out vec2 vtexCoord;"
      "void main() {"
      "  gl_Position = modelViewProjectionMatrix * vec4(vertex, 1.);"
      "  vtexCoord = (4.0/radius)*(vertex.xy+vertex.zx);"
      "}";
    vs = new QOpenGLShader(QOpenGLShader::Vertex, this);
    vs->compileSourceCode(vs_src);

	// FS
    fs = new QOpenGLShader(QOpenGLShader::Fragment, this);
    fs->compileSourceFile("plugins/multitex/splatting.frag");

	// Program  
  	program = new QOpenGLShaderProgram(this);
    program->addShader(vs);
	program->addShader(fs);
	program->link();

	// Load Texture 1
	g.glActiveTexture(GL_TEXTURE0);
    QString filename = QFileDialog::getOpenFileName(0, "Open Image", "plugins/multitex", "Image file (*.png *.jpg)");
	QImage img0(filename);	
	QImage im0 = img0.convertToFormat(QImage::Format_ARGB32).rgbSwapped().mirrored();
	g.glGenTextures( 1, &textureId0);
	g.glBindTexture(GL_TEXTURE_2D, textureId0);
	g.glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, im0.width(), im0.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, im0.bits());
	g.glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	g.glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	g.glBindTexture(GL_TEXTURE_2D, 0);

	// Load Texture 2
	g.glActiveTexture(GL_TEXTURE1);
    QString filename2 = QFileDialog::getOpenFileName(0, "Open Image 2", "plugins/multitex", "Image file (*.png *.jpg)");
	QImage img1(filename2);	
	QImage im1 = img1.convertToFormat(QImage::Format_ARGB32).rgbSwapped().mirrored();
	g.glGenTextures( 1, &textureId1);
	g.glBindTexture(GL_TEXTURE_2D, textureId1);
	g.glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, im1.width(), im1.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, im1.bits());
	g.glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	g.glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	g.glBindTexture(GL_TEXTURE_2D, 0);

    g.glActiveTexture(GL_TEXTURE2);
    QString filename3 = QFileDialog::getOpenFileName(0, "Open noise image", "plugins/multitex", "Image file (*.png *.jpg)");
    QImage img2(filename3);
    QImage im2 = img2.convertToFormat(QImage::Format_ARGB32).rgbSwapped().mirrored();
    g.glGenTextures( 1, &textureId2);
    g.glBindTexture(GL_TEXTURE_2D, textureId2);
    g.glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, im2.width(), im2.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, im2.bits());
    g.glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    g.glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    g.glBindTexture(GL_TEXTURE_2D, 0);
}

void Multitex::preFrame() {
    GLWidget &g = *glwidget();
    g.makeCurrent();

    // bind shader and define uniforms
    program->bind();
    program->setUniformValue("sampler0", 0);  // texture unit del primer sampler
    program->setUniformValue("sampler1", 1);  // texture unit del segon  sampler
    program->setUniformValue("noise", 2);  // texture unit del segon  sampler
    program->setUniformValue("radius", float(scene()->boundingBox().radius()));  // radi d'una esfera que engloba l'escena
    program->setUniformValue("modelViewProjectionMatrix", g.camera()->projectionMatrix() * g.camera()->viewMatrix());
    // bind textures
    g.glActiveTexture(GL_TEXTURE0);
    g.glBindTexture(GL_TEXTURE_2D, textureId0);
    g.glActiveTexture(GL_TEXTURE1);
    g.glBindTexture(GL_TEXTURE_2D, textureId1);
    g.glActiveTexture(GL_TEXTURE2);
    g.glBindTexture(GL_TEXTURE_2D, textureId2);
}

void Multitex::postFrame() 
{
    GLWidget &g = *glwidget();
    g.makeCurrent();

    // unbind shader
    program->release();
    // unbind textures
    g.glActiveTexture(GL_TEXTURE0);
    g.glBindTexture(GL_TEXTURE_2D, 0);
    g.glActiveTexture(GL_TEXTURE1);
    g.glBindTexture(GL_TEXTURE_2D, 0);
    g.glActiveTexture(GL_TEXTURE1);
    g.glBindTexture(GL_TEXTURE_2D, 0);
}

