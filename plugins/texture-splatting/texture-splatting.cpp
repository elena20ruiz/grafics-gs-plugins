#include "texture-splatting.h"
#include <QFileDialog>
 
void TextureSplatting::onPluginLoad(){
	// VS + FS
  vs=new QGLShader(QGLShader::Vertex, this);
  vs->compileSourceFile("/home/llop/Llop/FIB/2015-2016QT/G/Lab/NewViewer/plugins/texture-splatting/texture-splatting.vert");
  fs=new QGLShader(QGLShader::Fragment, this);
  fs->compileSourceFile("/home/llop/Llop/FIB/2015-2016QT/G/Lab/NewViewer/plugins/texture-splatting/texture-splatting.frag");
  
	// Program  
	program=new QGLShaderProgram(this);
  program->addShader(vs);
	program->addShader(fs);
	program->link();

	// Load noise
	glActiveTexture(GL_TEXTURE0);
	QString noiseFilename = QFileDialog::getOpenFileName(0, "Open Noise Image", "/assig/grau-g/Textures", "Image file (*.png *.jpg)");	
	QImage img3(noiseFilename);	
	QImage im3=QGLWidget::convertToGLFormat(img3);
	glGenTextures( 1, &noiseId);
	glBindTexture(GL_TEXTURE_2D, noiseId);
	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, im3.width(), im3.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, im3.bits());
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glBindTexture(GL_TEXTURE_2D, 0);

	// Load Texture 1
	glActiveTexture(GL_TEXTURE0);
	QString filename = QFileDialog::getOpenFileName(0, "Open Rock Image", "/assig/grau-g/Textures", "Image file (*.png *.jpg)");	
	QImage img0(filename);	
	QImage im0 = QGLWidget::convertToGLFormat(img0);
	glGenTextures( 1, &textureId0);
	glBindTexture(GL_TEXTURE_2D, textureId0);
	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, im0.width(), im0.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, im0.bits());
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glBindTexture(GL_TEXTURE_2D, 0);

	// Load Texture 2
	glActiveTexture(GL_TEXTURE1);
	QString filename2 = QFileDialog::getOpenFileName(0, "Open Grass Image", "/assig/grau-g/Textures", "Image file (*.png *.jpg)");	
	QImage img1(filename2);	
	QImage im1 = QGLWidget::convertToGLFormat(img1);
	glGenTextures( 1, &textureId1);
	glBindTexture(GL_TEXTURE_2D, textureId1);
	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, im1.width(), im1.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, im1.bits());
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glBindTexture(GL_TEXTURE_2D, 0);
}

void TextureSplatting::preFrame() {
  // bind shader and define uniforms
  program->bind();
  program->setUniformValue("noise0", 0);    // texture unit del noise 
  program->setUniformValue("rock1", 1);     // texture unit del primer sampler 
  program->setUniformValue("grass2", 2);    // texture unit del segon sampler 
  QMatrix4x4 MVP=camera()->projectionMatrix()*camera()->modelviewMatrix();
  program->setUniformValue("modelViewProjectionMatrix", MVP); 
  program->setUniformValue("normalMatrix", camera()->modelviewMatrix().normalMatrix());
  // bind textures
	glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, noiseId);
	glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, textureId0);
	glActiveTexture(GL_TEXTURE2);
  glBindTexture(GL_TEXTURE_2D, textureId1);
}

void TextureSplatting::postFrame() {
  // unbind shader
  program->release();
 	// unbind textures
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture(GL_TEXTURE2);
  glBindTexture(GL_TEXTURE_2D, 0);
}

Q_EXPORT_PLUGIN2(texture-splatting, TextureSplatting)   // plugin name, plugin class
