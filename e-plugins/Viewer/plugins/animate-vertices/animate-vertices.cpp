#include "animate-vertices.h"

void AnimateVertices::onPluginLoad() {
  // Carregar shader, compile & link 
  QString vs_src="#version 330 core\nlayout (location=0) in vec3 vertex; layout (location=1) in vec3 normal; layout (location=2) in vec3 color; layout (location=3) in vec2 texCoord; out vec4 frontColor; out vec2 vtexCoord; uniform mat4 modelViewProjectionMatrix; uniform mat3 normalMatrix; uniform float time; uniform float amplitude=0.1; uniform float freq=1; const float PI=acos(-1.0); void main() { vec3 V=vertex+normal*abs(amplitude*sin(PI*2*freq*time)); vec3 N=normalize(normalMatrix*normal); frontColor=vec4(vec3(N.z),1); gl_Position=modelViewProjectionMatrix*vec4(V, 1); }";
  vs=new QGLShader(QGLShader::Vertex, this);
  vs->compileSourceCode(vs_src);
  //cout << "VS log:" << vs->log().toStdString() << endl;
  QString fs_src="#version 330 core\nin vec4 frontColor; out vec4 fragColor; void main() { fragColor=frontColor; }";
  fs = new QGLShader(QGLShader::Fragment, this);
  fs->compileSourceCode(fs_src);
  //cout << "FS log:" << fs->log().toStdString() << endl;
  program = new QGLShaderProgram(this);
  program->addShader(vs);
  program->addShader(fs);
  program->link();
  //cout << "Link log:" << program->log().toStdString() << endl;
  //start timer
  elapsedTimer.start();
  // keep repainting
  QTimer *timer = new QTimer(this);
  connect(timer, SIGNAL(timeout()), glwidget(), SLOT(updateGL()));
  timer->start();
}

void AnimateVertices::preFrame() {
  // bind shader and define uniforms
  program->bind();
  program->setUniformValue("time", float(elapsedTimer.elapsed()/1000.0f));
  QMatrix3x3 NM=camera()->modelviewMatrix().normalMatrix();
  program->setUniformValue("normalMatrix", NM); 
  QMatrix4x4 MVP=camera()->projectionMatrix()*camera()->modelviewMatrix();
  program->setUniformValue("modelViewProjectionMatrix", MVP); 
}

void AnimateVertices::postFrame() {
  // unbind shader
  program->release();
}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(animate-vertices, AnimateVertices)   // plugin name, plugin class
#endif

