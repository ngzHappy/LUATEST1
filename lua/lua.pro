
CONFIG += c++14
CONFIG += console

QT += core

TARGET = lua
TEMPLATE = app

SOURCES += main.cpp


INCLUDEPATH += $$PWD/../QLuaCore

include( $$PWD/../BuildPaths.pri )
DESTDIR = $$BUILD_PATH_
LIBS += -L$$BUILD_PATH_ -lQLuaCore


