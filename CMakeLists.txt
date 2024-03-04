cmake_minimum_required(VERSION 3.20)

project (MayaAPIBuildAllDemos)
set(PLUGINS_FOLDER ${CMAKE_CURRENT_SOURCE_DIR}/plugins)

add_subdirectory(Lecture1/Hello)
add_subdirectory(Lecture1/CustomSphere)
add_subdirectory(CustomSphereSolution)
add_subdirectory(Lecture2/)
add_subdirectory(Lecture3/)


# Copy the python files from the PythonPlugins folder to the plugins folder
file(GLOB PYTHON_PLUGINS ${CMAKE_CURRENT_SOURCE_DIR}/PythonPlugins/*.py)
file(COPY ${PYTHON_PLUGINS} DESTINATION ${PLUGINS_FOLDER})
# add to the IDE none of these get built
add_custom_target(PythonPluginsForIDE SOURCES ${PYTHON_PLUGINS})

