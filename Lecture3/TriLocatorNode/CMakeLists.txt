cmake_minimum_required(VERSION 3.1)
project(triLocator)
# include the project setting file
include($ENV{DEVKIT_LOCATION}/cmake/pluginEntry.cmake)


# set SOURCE_FILES
set(SOURCE_FILES   src/Plugin.cpp 
                   src/TriLocatorNode.cpp 
                   src/TriLocatorNodeData.cpp 
                   src/TriLocatorNodeDrawOverride.cpp
                   include/TriLocatorNode.h 
                   include/TriLocatorNodeData.h 
                   include/TriLocatorNodeDrawOverride.h
                                      
                   
                   )
include_directories(include)
# set linking libraries
set(LIBRARIES OpenMaya OpenMayaRender OpenMayaUI Foundation )
# This function is from the MayaAPI cmake files and get the platform specific includes for OpenGL
find_opengl()

# Build plugin
build_plugin()
