#ifndef UNIVERSALSEARCHER_GLOBAL_H
#define UNIVERSALSEARCHER_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(UNIVERSALSEARCHER_LIBRARY)
#  define UNIVERSALSEARCHERSHARED_EXPORT Q_DECL_EXPORT
#else
#  define UNIVERSALSEARCHERSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // UNIVERSALSEARCHER_GLOBAL_H