cmake_minimum_required(VERSION 3.0)

project(spryengine)

add_executable(spryengine main.cpp)

install(TARGETS spryengine RUNTIME DESTINATION bin)
