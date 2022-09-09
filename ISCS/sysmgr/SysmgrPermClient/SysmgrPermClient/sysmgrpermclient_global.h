#ifndef SYSMGRPERMCLIENT_GLOBAL_H
#define SYSMGRPERMCLIENT_GLOBAL_H

//#include <QtCore/qglobal.h>

#if defined(SYSMGRPERMCLIENT_LIBRARY)
#  define SYSMGRPERMCLIENTSHARED_EXPORT __declspec(dllexport)
#else
#  define SYSMGRPERMCLIENTSHARED_EXPORT __declspec(dllexport)
#endif

#endif // SYSMGRPERMCLIENT_GLOBAL_H
