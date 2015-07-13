#ifndef _9FOLDV8_GLOBAL_H
#define _9FOLDV8_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(_9FOLDV8_LIBRARY)
#  define _9FOLDV8SHARED_EXPORT Q_DECL_EXPORT
#else
#  define _9FOLDV8SHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // _9FOLDV8_GLOBAL_H
