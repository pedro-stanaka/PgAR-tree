#ifndef RTREEPG_GLOBAL_H
#define RTREEPG_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(RTREEPG_LIBRARY)
#  define RTREEPGSHARED_EXPORT Q_DECL_EXPORT
#else
#  define RTREEPGSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // RTREEPG_GLOBAL_H
