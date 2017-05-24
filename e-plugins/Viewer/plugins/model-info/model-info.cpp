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
  //1. Objectes: Càlcul directe
  nobj=scn->objects().size();

  npol=nver=ntri=0;
  int nver_face;


  for (int i=0; i<(int)scn->objects().size(); ++i) {
      //Per a cada objecte de l'escena
        const Object &obj = scn->objects()[i];

    for (int j=0; j<(int)obj.faces().size(); ++j) {
        //Per a cada cara de l'escena
          const Face &face = obj.faces()[j];

        //2. Polígon:
        ++npol;

        //3. Número de vertex del polígon
        nver_face = face.numVertices();
        nver+=nver_face;

        //4. Núm de polígons trianges
        ntri+=(nver_face==3);
    }
  }

  // PRINT
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

