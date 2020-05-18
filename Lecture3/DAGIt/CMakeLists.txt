cmake_minimum_required(VERSION 3.1)


project(DAGIt)
# include the project setting file
include($ENV{DEVKIT_LOCATION}/cmake/pluginEntry.cmake)

# specify project name
set(PROJECT_NAME DAGIt)

# set SOURCE_FILES
set(SOURCE_FILES   DAGIt.cpp)

# set linking libraries
set(LIBRARIES OpenMaya     Foundation)
# Build plugin
build_plugin()
