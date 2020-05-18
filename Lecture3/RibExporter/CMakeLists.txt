cmake_minimum_required(VERSION 3.1)


project(RibExport)
# include the project setting file
include($ENV{DEVKIT_LOCATION}/cmake/pluginEntry.cmake)

# specify project name
set(PROJECT_NAME RibExport)
include_directories(include)
# set SOURCE_FILES
set(SOURCE_FILES   src/Plugin.cpp src/RibExport.cpp include/RibExport.h)

# set linking libraries
set(LIBRARIES OpenMaya OpenMayaAnim OpenMayaRender Foundation)
# Build plugin
build_plugin()
