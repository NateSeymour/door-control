cmake_minimum_required(VERSION 4.0)
project(door_control C)

set(CMAKE_C_STANDARD 23)

set(PICO_SDK_FETCH_FROM_GIT ON)
include(cmake/pico.cmake)

add_executable(door_control main.c)
