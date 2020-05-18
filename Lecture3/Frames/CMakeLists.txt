cmake_minimum_required(VERSION 3.1)


project(TimeLine)
# include the project setting file
include($ENV{DEVKIT_LOCATION}/cmake/pluginEntry.cmake)

# specify project name
set(PROJECT_NAME TimeLine)

# set SOURCE_FILES
set(SOURCE_FILES   TimeLine.cpp)

# set linking libraries
set(LIBRARIES OpenMaya OpenMayaAnim Foundation)
# Build plugin
build_plugin()
