cmake_minimum_required(VERSION 3.1)


project(GetSelection)
# include the project setting file
include($ENV{DEVKIT_LOCATION}/cmake/pluginEntry.cmake)

# specify project name
set(PROJECT_NAME GetSelection)

# set SOURCE_FILES
set(SOURCE_FILES   GetSelection.cpp)

# set linking libraries
set(LIBRARIES OpenMaya  Foundation)
# Build plugin
build_plugin()
