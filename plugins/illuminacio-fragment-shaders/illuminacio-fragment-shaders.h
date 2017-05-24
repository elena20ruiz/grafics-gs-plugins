#ifndef _ILLUMINACIOFRAGMENTSHADERS_H  
#define _ILLUMINACIOFRAGMENTSHADERS_H

#include "basicplugin.h"

class IlluminacioFragmentShaders : public QObject, public BasicPlugin {

  Q_OBJECT
#if QT_VERSION >= 0x050000
  Q_PLUGIN_METADATA(IID "BasicPlugin")   
#endif
  Q_INTERFACES(BasicPlugin)

  QGLShaderProgram* program;
  QGLShader* vs;
  QGLShader* fs;

public:
  void onPluginLoad();
  void preFrame();
  void postFrame();
};

#endif


