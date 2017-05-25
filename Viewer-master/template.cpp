#include "template.h"
#include <cstdlib>

// Documentation: http://www.cs.upc.edu/~virtual/Gdocs/interfaces/html/class_basic_plugin.html


#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(alpha-blending, AlphaBlending)   // plugin name, plugin class
#endif

