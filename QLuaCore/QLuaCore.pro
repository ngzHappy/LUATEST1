
QT　+=core
QT  -= gui
CONFIG += c++14

TARGET = QLuaCore
TEMPLATE = lib

DEFINES += QLUACORE_LIBRARY

SOURCES += QLuaCore.cpp
SOURCES += VirtualBase.cpp

HEADERS += $$PWD/QLuaCore.hpp
HEADERS += $$PWD/LuaCoreDetail.hpp
HEADERS += $$PWD/qluacore_global.hpp

include( $$PWD/lua.pri )
include( $$PWD/../BuildPaths.pri )

HEADERS += $$PWD/VirtualBase.hpp
DESTDIR = $$BUILD_PATH_
