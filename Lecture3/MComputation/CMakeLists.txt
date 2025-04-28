cmake_minimum_required(VERSION 3.20)


project(MComputation)
# include the project setting file
include(${DEVKIT_LOCATION}/cmake/pluginEntry.cmake)

# specify project name
set(PROJECT_NAME MComputation)

# set SOURCE_FILES
set(SOURCE_FILES   MComputation.cpp)

# set linking libraries
set(LIBRARIES OpenMaya  Foundation)
# Build plugin
build_plugin()
# now copy the plugin to the local plugins folder set by PLUGINS_FOLDER
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${PROJECT_NAME}> ${PLUGINS_FOLDER}/${OutputName}
)
