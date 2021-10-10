cmake_minimum_required(VERSION 3.15)
project(ConwayGame)

set(CMAKE_CXX_STANDARD 11)
set(SOURCE_FILES ${CMAKE_SOURCE_DIR}/ConwayGame.cpp)

add_executable(${PROJECT_NAME}${SOURCE_FILES})