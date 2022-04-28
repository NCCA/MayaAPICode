cmake_minimum_required(VERSION 3.1)
project(QtGUICPP)
# include the project setting file
include($ENV{DEVKIT_LOCATION}/cmake/pluginEntry.cmake)


# set SOURCE_FILES
set(SOURCE_FILES   src/QtGui.cpp src/Plugin.cpp include/QtGui.h )
include_directories(include)
# set linking libraries
set(LIBRARIES OpenMaya     Foundation)
# Build plugin
build_plugin()
