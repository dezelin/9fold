#ifndef _9FOLDFFORMATS_GLOBAL_H
#define _9FOLDFFORMATS_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(_9FOLDFFORMATS_LIBRARY)
#  define _9FOLDFFORMATSSHARED_EXPORT Q_DECL_EXPORT
#else
#  define _9FOLDFFORMATSSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // _9FOLDFFORMATS_GLOBAL_H
