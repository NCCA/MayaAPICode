cmake_minimum_required(VERSION 3.1)
project(HelloMaya)
# include the project setting file
include(${DEVKIT_LOCATION}/devkitBase/cmake/pluginEntry.cmake)

# specify project name
set(PROJECT_NAME HelloMaya)

# set SOURCE_FILES
set(SOURCE_FILES   HelloMaya.cpp)

# set linking libraries
set(LIBRARIES OpenMaya     Foundation)
# Build plugin
build_plugin()
