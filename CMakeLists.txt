cmake_minimum_required(VERSION 2.8.11)

project(renameosgb)

find_package(OpenSceneGraph 3.1.3 REQUIRED osgDB osgUtil)
include_directories(include ${OPENSCENEGRAPH_INCLUDE_DIRS})

add_executable(renameosgb
  source/main.cpp
  source/renameosgb.cpp)

target_link_libraries(renameosgb ${OPENSCENEGRAPH_LIBRARIES})