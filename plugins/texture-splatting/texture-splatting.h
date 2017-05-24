#ifndef _TEXTURE_SPLATTING_H
#define _TEXTURE_SPLATTING_H

#include "basicplugin.h"
#include <QGLShader>
#include <QGLShaderProgram>


class TextureSplatting : public QObject, public BasicPlugin {
  Q_OBJECT
  Q_INTERFACES(BasicPlugin)

public:
  void onPluginLoad();
  void preFrame();
  void postFrame();
    
private:
  QGLShaderProgram* program;
  QGLShader* vs;
  QGLShader* fs; 
  GLuint noiseId, textureId0, textureId1;
};
 
#endif
