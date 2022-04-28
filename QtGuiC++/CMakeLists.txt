cmake_minimum_required(VERSION 3.1)
project(QtGUICPP)
# include the project setting file
include($ENV{DEVKIT_LOCATION}/cmake/pluginEntry.cmake)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5 COMPONENTS Widgets)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# set SOURCE_FILES
set(SOURCE_FILES   src/QtGui.cpp src/Plugin.cpp include/QtGui.h )
include_directories(include)
# set linking libraries
set(LIBRARIES OpenMaya     Foundation Qt5::Widgets )
# Build plugin
build_plugin()
