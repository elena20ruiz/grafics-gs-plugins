#ifndef _FRAMERATE_H  
#define _FRAMERATE_H

#include "basicplugin.h"

class Framerate : public QObject, public BasicPlugin {

  Q_OBJECT
#if QT_VERSION >= 0x050000
  Q_PLUGIN_METADATA(IID "BasicPlugin")   
#endif
  Q_INTERFACES(BasicPlugin)

  void drawRect();

  int fcnt;
  int fps;

  GLuint textureID;
  QGLShaderProgram* program;
  QGLShader* vs;
  QGLShader* fs;

public:
  void onPluginLoad();
  void postFrame();

public slots:
  void updateFPS();

};

#endif


