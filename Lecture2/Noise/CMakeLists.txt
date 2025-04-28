cmake_minimum_required(VERSION 3.20)
project(noiseNode)
set(CMAKE_CXX_STANDARD 17)
# include the project setting file
include(${DEVKIT_LOCATION}/cmake/pluginEntry.cmake)


# set SOURCE_FILES
set(SOURCE_FILES   src/NoiseNode.cpp src/Noise.cpp src/Plugin.cpp include/Noise.h include/NoiseNode.h )
include_directories(include)
# set linking libraries
set(LIBRARIES OpenMaya Foundation)
# Build plugin
build_plugin()

# now copy the plugin to the local plugins folder set by PLUGINS_FOLDER
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${PROJECT_NAME}> ${PLUGINS_FOLDER}/${OutputName}
)
