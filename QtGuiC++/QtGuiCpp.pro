include(qtconfig)

QT += widgets
unix|macx {
  CONFIG += no_plugin_name_prefix
}
CONFIG+=Release
INCLUDEPATH+=include
TARGET = QtGuiCPP
HEADERS += include/QtGui.h
SOURCES += src/QtGui.cpp \
           src/Plugin.cpp
