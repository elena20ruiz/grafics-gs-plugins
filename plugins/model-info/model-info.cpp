#include "model-info.h"
#include "glwidget.h"
#include <cstdio>

// número total d’objectes carregats, 
// número total de polígons, 
// número total de vèrtexs, 
// i el percentatge de polígons que són triangles
void ModelInfo::printModelInfo() {
  // do count
  Scene* scn=scene();
  int nobj, npol, nver, ntri;
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
  // print model info
  printf("Numero objectes: %d\n", nobj);
  printf("Numero poligons: %d\n", npol);
  printf("Numero vertex: %d\n", nver);
  printf("Percentatge triangles: %f\n\n", ((double)ntri)/npol);
}

void ModelInfo::onPluginLoad() {
  printModelInfo();
}

void ModelInfo::onObjectAdd() {
  printModelInfo();
}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(model-info, ModelInfo)   // plugin name, plugin class
#endif

