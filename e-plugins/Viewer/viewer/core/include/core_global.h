// See Qt Reference documentation, "Creating Shared Libraries"
#ifndef _CORE_GLOBAL_H_
#define _CORE_GLOBAL_H_

#include <QtCore/QtGlobal>

#if defined(CORE_LIBRARY)
#  define CORE_EXPORT Q_DECL_EXPORT
#else
#  define CORE_EXPORT Q_DECL_IMPORT
#endif

#endif

