cmake_minimum_required(VERSION 3.1)
project(HelloMaya)
# include the project setting file
include($ENV{DEVKIT_LOCATION}/cmake/pluginEntry.cmake)

# specify project name
set(PROJECT_NAME HelloMaya)

# set SOURCE_FILES
set(SOURCE_FILES   HelloMaya.cpp)

# set linking libraries
set(LIBRARIES OpenMaya     Foundation)
# Build plugin
build_plugin()

install(TARGETS ${PROJECT_NAME} DESTINATION ~/Library/Preferences/Autodesk/maya/2022/plug-ins)

#install(DIRECTORY   ${CMAKE_SOURCE_DIR}/include/ngl/ DESTINATION include/ngl)
#install(FILES   ${CMAKE_SOURCE_DIR}/gl3w/gl3w.h DESTINATION include/ngl)

