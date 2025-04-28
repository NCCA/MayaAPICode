cmake_minimum_required(VERSION 3.20)
project(CustomSphereSolution)
# include the project setting file
include(${DEVKIT_LOCATION}/cmake/pluginEntry.cmake)


# set SOURCE_FILES
set(SOURCE_FILES   src/CustomSphere.cpp src/Plugin.cpp include/CustomSphere.h )
include_directories(include)
# set linking libraries
set(LIBRARIES OpenMaya     Foundation)
# Build plugin
build_plugin()

# now copy the plugin to the local plugins folder set by PLUGINS_FOLDER
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${PROJECT_NAME}> ${PLUGINS_FOLDER}/${OutputName}
)
