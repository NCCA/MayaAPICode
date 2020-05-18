cmake_minimum_required(VERSION 3.1)
project(sineNode)
set(CMAKE_CXX_STANDARD 17)
# include the project setting file
include($ENV{DEVKIT_LOCATION}/cmake/pluginEntry.cmake)


# set SOURCE_FILES
set(SOURCE_FILES   src/SineNode.cpp  src/Plugin.cpp include/SineNode.h )
include_directories(include)
# set linking libraries
set(LIBRARIES OpenMaya Foundation)
# Build plugin
build_plugin()
