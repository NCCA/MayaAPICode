cmake_minimum_required(VERSION 3.20)


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
# now copy the plugin to the local plugins folder set by PLUGINS_FOLDER
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${PROJECT_NAME}> ${PLUGINS_FOLDER}/${OutputName}
)
