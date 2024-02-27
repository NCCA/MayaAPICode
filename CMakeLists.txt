cmake_minimum_required(VERSION 3.20)

project (MayaAPIBuildAllDemos)
set(PLUGINS_FOLDER ${CMAKE_CURRENT_SOURCE_DIR}/plugins)

add_subdirectory(Lecture1/Hello)
add_subdirectory(Lecture1/CustomSphere)
add_subdirectory(CustomSphereSolution)
add_subdirectory(Lecture2/)
add_subdirectory(Lecture3/)



