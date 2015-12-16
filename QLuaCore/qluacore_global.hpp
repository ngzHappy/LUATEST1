#if !defined( QLUACORE_GLOBAL_HPP )
#define QLUACORE_GLOBAL_HPP

#include <memory>
#include <QtCore/qglobal.h>

#if defined(QLUACORE_LIBRARY)
#  define QLUACORESHARED_EXPORT Q_DECL_EXPORT
#else
#  define QLUACORESHARED_EXPORT Q_DECL_IMPORT
#endif

//LUA_USE_WINDOWS
//LUA_USE_LINUX
//LUA_USE_MACOSX

#if defined(Q_OS_WIN)
//lua config has defined some thing
#else
#define LUA_USE_LINUX
#endif

#endif // QLUACORE_GLOBAL_HPP
