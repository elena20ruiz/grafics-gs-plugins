#ifndef _MODELINFO_H  
#define _MODELINFO_H

#include "basicplugin.h"

class ModelInfo : public QObject, public BasicPlugin {

  Q_OBJECT
#if QT_VERSION >= 0x050000
  Q_PLUGIN_METADATA(IID "BasicPlugin")   
#endif
  Q_INTERFACES(BasicPlugin)

  void printModelInfo();

public:
  void onPluginLoad();
  void onObjectAdd();
};

#endif


