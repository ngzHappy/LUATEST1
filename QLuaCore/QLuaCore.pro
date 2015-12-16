
QT　+=core
QT  -= gui
CONFIG += c++14

TARGET = QLuaCore
TEMPLATE = lib

DEFINES += QLUACORE_LIBRARY

SOURCES += QLuaCore.cpp

HEADERS += $$PWD/QLuaCore.hpp
HEADERS += $$PWD/qluacore_global.hpp

include( $$PWD/lua.pri )

HEADERS += $$PWD/VirtualBase.hpp

