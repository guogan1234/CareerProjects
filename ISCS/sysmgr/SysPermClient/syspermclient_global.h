#ifndef SYSPERMCLIENT_GLOBAL_H
#define SYSPERMCLIENT_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(SYSPERMCLIENT_LIBRARY)
#  define SYSPERMCLIENTSHARED_EXPORT Q_DECL_EXPORT
#else
#  define SYSPERMCLIENTSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // SYSPERMCLIENT_GLOBAL_H
