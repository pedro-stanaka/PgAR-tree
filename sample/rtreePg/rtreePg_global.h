#ifndef RTREEPG_GLOBAL_H
#define RTREEPG_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(RTREEPG_LIBRARY)
#  define RTREEPGSHARED_EXPORT Q_DECL_EXPORT
#else
#  define RTREEPGSHARED_EXPORT Q_DECL_IMPORT
#endif

#define INDEX_FILE "/var/lib/postgresql/artree.dat"
#define PAGE_SIZE  1024

typedef struct {
  char ** data;
  int size;
} compositeArray;

typedef struct {
    int block;
    int position;
} tuplePtr;

#endif // RTREEPG_GLOBAL_H
