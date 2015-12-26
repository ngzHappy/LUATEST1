TEMPLATE = subdirs

#.subdir	Use the specified subdirectory instead of SUBDIRS value.
#.file	Specify the subproject pro file explicitly. Cannot be used in conjunction with .subdir modifier.
#.depends	This subproject depends on specified subproject. Available only on platforms that use makefiles.
#.makefile	The makefile of subproject. Available only on platforms that use makefiles.
#.target	Base string used for makefile targets related to this subproject. Available only on platforms that use makefiles.

QLuaCore.file = $$PWD/QLuaCore/QLuaCore.pro
TestBase.file = $$PWD/TestBase/TestBase.pro
TestQVariant.file = $$PWD/TestQVariant/TestQVariant.pro
TestSaveTable.file = $$PWD/TestSaveTable/TestSaveTable.pro
lua.file = $$PWD/lua/lua.pro

TestBase.depends += QLuaCore
TestQVariant.depends += QLuaCore
TestSaveTable.depends += QLuaCore
lua.depends += QLuaCore

SUBDIRS += QLuaCore
SUBDIRS += lua
SUBDIRS += TestSaveTable
SUBDIRS += TestQVariant
SUBDIRS += TestBase




